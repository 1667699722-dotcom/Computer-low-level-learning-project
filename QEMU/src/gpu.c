#include "include/gpu.h"
#include "include/uart.h"
#include "include/lib.h"

// ==================== Virtqueue 描述符结构 ====================
struct virtq_desc {
    uint64_t addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
};

#define VRING_DESC_F_NEXT               1
#define VRING_DESC_F_WRITE              2

// ==================== Available Ring ====================
struct virtq_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[];
};

// ==================== Used Ring 元素 ====================
struct virtq_used_elem {
    uint32_t id;
    uint32_t len;
};

// ==================== Used Ring ====================
struct virtq_used {
    uint16_t flags;
    uint16_t idx;
    struct virtq_used_elem ring[];
};

// ==================== Virtio MMIO 寄存器偏移 ====================
#define VIRTIO_MMIO_MAGIC_VALUE         0x000
#define VIRTIO_MMIO_VERSION             0x004
#define VIRTIO_MMIO_DEVICE_ID           0x008
#define VIRTIO_MMIO_VENDOR_ID           0x00c
#define VIRTIO_MMIO_DEVICE_FEATURES     0x010
#define VIRTIO_MMIO_DEVICE_FEATURES_SEL 0x014
#define VIRTIO_MMIO_DRIVER_FEATURES     0x020
#define VIRTIO_MMIO_DRIVER_FEATURES_SEL 0x024
#define VIRTIO_MMIO_QUEUE_SEL           0x030
#define VIRTIO_MMIO_QUEUE_NUM_MAX       0x034
#define VIRTIO_MMIO_QUEUE_NUM           0x038
#define VIRTIO_MMIO_QUEUE_READY         0x044
#define VIRTIO_MMIO_QUEUE_NOTIFY        0x050
#define VIRTIO_MMIO_INTERRUPT_STATUS    0x060
#define VIRTIO_MMIO_INTERRUPT_ACK       0x064
#define VIRTIO_MMIO_STATUS              0x070
#define VIRTIO_MMIO_QUEUE_DESC_LOW      0x080
#define VIRTIO_MMIO_QUEUE_DESC_HIGH     0x084
#define VIRTIO_MMIO_QUEUE_AVAIL_LOW     0x090
#define VIRTIO_MMIO_QUEUE_AVAIL_HIGH    0x094
#define VIRTIO_MMIO_QUEUE_USED_LOW      0x0a0
#define VIRTIO_MMIO_QUEUE_USED_HIGH     0x0a4

// ==================== 状态位 ====================
#define VIRTIO_CONFIG_S_ACKNOWLEDGE     1
#define VIRTIO_CONFIG_S_DRIVER          2
#define VIRTIO_CONFIG_S_FEATURES_OK     8
#define VIRTIO_CONFIG_S_DRIVER_OK       4

// ==================== 驱动全局变量 ====================
#define QUEUE_SIZE 8
static struct virtq_desc* desc;
static struct virtq_avail* avail;
static struct virtq_used* used;
static uint16_t used_idx;
static uint8_t free_desc[QUEUE_SIZE];
static uint64_t virtio_gpu_mmio_base = 0;

// ==================== GPU 全局状态 ====================
uint32_t gpu_width = 0;
uint32_t gpu_height = 0;
uint32_t* gpu_framebuffer = 0;

// 命令/响应缓冲区
static struct virtio_gpu_ctrl_hdr resp_buf;

// ==================== MMIO 寄存器访问宏 ====================
#define VIRTIO_REG(reg) (*(volatile uint32_t*)(virtio_gpu_mmio_base + (reg)))

// ==================== 分配描述符 ====================
static int alloc_desc() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        if(free_desc[i]) {
            free_desc[i] = 0;
            return i;
        }
    }
    return -1;
}

// ==================== 释放描述符链 ====================
static void free_desc_chain(int idx) {
    while(idx != -1) {
        int next = desc[idx].next;
        free_desc[idx] = 1;
        if(desc[idx].flags & VRING_DESC_F_NEXT) {
            idx = next;
        } else {
            break;
        }
    }
}

// ==================== 发送控制命令并等待响应 ====================
static int send_ctrl_cmd(void* cmd, uint32_t cmd_len, void* resp, uint32_t resp_len) {
    // 分配两个描述符
    int idx0 = alloc_desc();
    int idx1 = alloc_desc();
    
    if(idx0 == -1 || idx1 == -1) {
        uart_puts("virtio-gpu: no free descriptors\n");
        return -1;
    }
    
    // 描述符 0: 命令（设备读）
    desc[idx0].addr = (uint64_t)cmd;
    desc[idx0].len = cmd_len;
    desc[idx0].flags = VRING_DESC_F_NEXT;
    desc[idx0].next = idx1;
    
    // 描述符 1: 响应（设备写）
    desc[idx1].addr = (uint64_t)resp;
    desc[idx1].len = resp_len;
    desc[idx1].flags = VRING_DESC_F_WRITE;
    desc[idx1].next = 0;
    
    // 添加到 Available Ring
    avail->ring[avail->idx % QUEUE_SIZE] = idx0;
    avail->idx++;
    
    // 通知设备
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_NOTIFY) = 0;
    
    // 等待设备处理（轮询）
    while(used_idx == used->idx);
    
    // 检查响应
    struct virtio_gpu_ctrl_hdr* resp_hdr = (struct virtio_gpu_ctrl_hdr*)resp;
    
    int success = (resp_hdr->type == VIRTIO_GPU_RESP_OK_NODATA || 
                   resp_hdr->type == VIRTIO_GPU_RESP_OK_DISPLAY_INFO);
    
    // 释放描述符
    free_desc_chain(idx0);
    used_idx++;
    
    return success ? 0 : -1;
}

// ==================== 查找 virtio-gpu 设备 ====================
static int find_virtio_gpu_device() {
    for(uint64_t base = 0x0a000000; base < 0x0a010000; base += 0x200) {
        volatile uint32_t* magic = (volatile uint32_t*)base;
        if(*magic != 0x74726976) continue;
        
        volatile uint32_t* dev_id = (volatile uint32_t*)(base + VIRTIO_MMIO_DEVICE_ID);
        if(*dev_id == VIRTIO_DEVICE_ID_GPU) {
            uart_puts("virtio-gpu: found at 0x");
            uart_put_hex(base);
            uart_puts("\n");
            virtio_gpu_mmio_base = base;
            return 0;
        }
    }
    uart_puts("virtio-gpu: device not found\n");
    return -1;
}

// ==================== 获取显示信息 ====================
static int get_display_info() {
    struct virtio_gpu_ctrl_hdr cmd;
    struct virtio_gpu_resp_display_info disp_info;
    
    // 清空缓冲区
    for(uint32_t i = 0; i < sizeof(cmd); i++) {
        ((uint8_t*)&cmd)[i] = 0;
    }
    for(uint32_t i = 0; i < sizeof(disp_info); i++) {
        ((uint8_t*)&disp_info)[i] = 0;
    }
    
    // 填充命令头
    cmd.type = VIRTIO_GPU_CMD_GET_DISPLAY_INFO;
    cmd.flags = 0;
    cmd.fence_id = 0;
    cmd.ctx_id = 0;
    cmd.ring_idx = 0;
    
    uart_puts("virtio-gpu: getting display info...\n");
    
    if(send_ctrl_cmd(&cmd, sizeof(cmd), 
                      &disp_info, sizeof(disp_info)) != 0) {
        uart_puts("virtio-gpu: get display info failed!\n");
        return -1;
    }
    
    // 调试输出所有pmodes信息
    uart_puts("virtio-gpu: display modes:\n");
    for(int i = 0; i < VIRTIO_GPU_MAX_SCANOUTS; i++) {
        uart_puts("  mode ");
        uart_put_dec(i);
        uart_puts(" enabled=");
        uart_put_dec(disp_info.pmodes[i].enabled);
        uart_puts(" size=");
        uart_put_dec(disp_info.pmodes[i].r.width);
        uart_puts("x");
        uart_put_dec(disp_info.pmodes[i].r.height);
        uart_puts("\n");
    }
    
    // 获取第一个可用的显示模式
    for(int i = 0; i < VIRTIO_GPU_MAX_SCANOUTS; i++) {
        if(disp_info.pmodes[i].enabled && disp_info.pmodes[i].r.width > 0 && disp_info.pmodes[i].r.height > 0) {
            gpu_width = disp_info.pmodes[i].r.width;
            gpu_height = disp_info.pmodes[i].r.height;
            
            uart_puts("virtio-gpu: display size ");
            uart_put_dec(gpu_width);
            uart_puts("x");
            uart_put_dec(gpu_height);
            uart_puts("\n");
            
            return 0;
        }
    }
    
    // 如果没有找到，用默认值
    gpu_width = 640;
    gpu_height = 480;
    uart_puts("virtio-gpu: using default 640x480\n");
    return 0;
}

// ==================== 创建 2D 资源 ====================
static int create_resource_2d(uint32_t resource_id, uint32_t width, uint32_t height) {
    struct virtio_gpu_resource_create_2d create_cmd;
    
    // 填充命令
    for(uint32_t i = 0; i < sizeof(create_cmd); i++) {
        ((uint8_t*)&create_cmd)[i] = 0;
    }
    create_cmd.hdr.type = VIRTIO_GPU_CMD_RESOURCE_CREATE_2D;
    create_cmd.hdr.flags = 0;
    create_cmd.hdr.fence_id = 0;
    create_cmd.hdr.ctx_id = 0;
    create_cmd.resource_id = resource_id;
    create_cmd.format = VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM;
    create_cmd.width = width;
    create_cmd.height = height;
    
    uart_puts("virtio-gpu: creating 2D resource...\n");
    
    if(send_ctrl_cmd(&create_cmd, sizeof(create_cmd), 
                      &resp_buf, sizeof(resp_buf)) != 0) {
        uart_puts("virtio-gpu: create resource failed!\n");
        return -1;
    }
    
    return 0;
}

// ==================== Attach Backing ====================
static int attach_backing(uint32_t resource_id, void* addr, uint32_t size) {
    // 临时缓冲区，包含 attach_cmd 和 entry
    struct {
        struct virtio_gpu_resource_attach_backing cmd;
        struct virtio_gpu_mem_entry entry;
    } attach_cmd_full;
    
    // 清空缓冲区
    for(uint32_t i = 0; i < sizeof(attach_cmd_full); i++) {
        ((uint8_t*)&attach_cmd_full)[i] = 0;
    }
    
    // 填充命令
    attach_cmd_full.cmd.hdr.type = VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING;
    attach_cmd_full.cmd.hdr.flags = 0;
    attach_cmd_full.cmd.hdr.fence_id = 0;
    attach_cmd_full.cmd.hdr.ctx_id = 0;
    attach_cmd_full.cmd.resource_id = resource_id;
    attach_cmd_full.cmd.nr_entries = 1;
    attach_cmd_full.entry.addr = (uint64_t)addr;
    attach_cmd_full.entry.length = size;
    
    uart_puts("virtio-gpu: attaching backing...\n");
    
    if(send_ctrl_cmd(&attach_cmd_full, sizeof(attach_cmd_full), 
                      &resp_buf, sizeof(resp_buf)) != 0) {
        uart_puts("virtio-gpu: attach backing failed!\n");
        return -1;
    }
    
    return 0;
}

// ==================== 设置 Scanout ====================
static int set_scanout(uint32_t scanout_id, uint32_t resource_id, 
                       uint32_t width, uint32_t height) {
    struct virtio_gpu_set_scanout scanout_cmd;
    
    // 填充命令
    for(uint32_t i = 0; i < sizeof(scanout_cmd); i++) {
        ((uint8_t*)&scanout_cmd)[i] = 0;
    }
    scanout_cmd.hdr.type = VIRTIO_GPU_CMD_SET_SCANOUT;
    scanout_cmd.hdr.flags = 0;
    scanout_cmd.hdr.fence_id = 0;
    scanout_cmd.hdr.ctx_id = 0;
    scanout_cmd.r.x = 0;
    scanout_cmd.r.y = 0;
    scanout_cmd.r.width = width;
    scanout_cmd.r.height = height;
    scanout_cmd.scanout_id = scanout_id;
    scanout_cmd.resource_id = resource_id;
    
    uart_puts("virtio-gpu: setting scanout...\n");
    
    if(send_ctrl_cmd(&scanout_cmd, sizeof(scanout_cmd), 
                      &resp_buf, sizeof(resp_buf)) != 0) {
        uart_puts("virtio-gpu: set scanout failed!\n");
        return -1;
    }
    
    return 0;
}

// ==================== Transfer 2D to Host ====================
static void virtio_gpu_transfer_to_host_2d()
{
	struct virtio_gpu_transfer_to_host_2d transfer_cmd;

	// 填充命令
	for(uint32_t i = 0; i < sizeof(transfer_cmd); i++) {
		((uint8_t*)&transfer_cmd)[i] = 0;
	}
	transfer_cmd.hdr.type = VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D;
	transfer_cmd.hdr.flags = 0;
	transfer_cmd.hdr.fence_id = 0;
	transfer_cmd.hdr.ctx_id = 0;
	transfer_cmd.r.x = 0;
	transfer_cmd.r.y = 0;
	transfer_cmd.r.width = gpu_width;
	transfer_cmd.r.height = gpu_height;
	transfer_cmd.offset = 0;
	transfer_cmd.resource_id = 1;

	send_ctrl_cmd(&transfer_cmd, sizeof(transfer_cmd),
	              &resp_buf, sizeof(resp_buf));
}

// ==================== Flush 刷新 ====================
void virtio_gpu_flush()
{
	// 先把像素数据从 Guest 内存传输到 Host 的 GPU 资源
	virtio_gpu_transfer_to_host_2d();

	struct virtio_gpu_resource_flush flush_cmd;

	// 填充命令
	for(uint32_t i = 0; i < sizeof(flush_cmd); i++) {
		((uint8_t*)&flush_cmd)[i] = 0;
	}
	flush_cmd.hdr.type = VIRTIO_GPU_CMD_RESOURCE_FLUSH;
	flush_cmd.hdr.flags = 0;
	flush_cmd.hdr.fence_id = 0;
	flush_cmd.hdr.ctx_id = 0;
	flush_cmd.r.x = 0;
	flush_cmd.r.y = 0;
	flush_cmd.r.width = gpu_width;
	flush_cmd.r.height = gpu_height;
	flush_cmd.resource_id = 1;

	send_ctrl_cmd(&flush_cmd, sizeof(flush_cmd),
	              &resp_buf, sizeof(resp_buf));
}

// ==================== 画像素 ====================
// color: 0xRRGGBB (Red, Green, Blue), 自动设 Alpha 为 0xFF
void virtio_gpu_draw_pixel(uint32_t x, uint32_t y, uint32_t color)
{
	if(x < gpu_width && y < gpu_height && gpu_framebuffer) {
		uint8_t* pixel = (uint8_t*)&gpu_framebuffer[y * gpu_width + x];
		pixel[0] = (color >> 0) & 0xFF;  // Blue
		pixel[1] = (color >> 8) & 0xFF;  // Green
		pixel[2] = (color >> 16) & 0xFF; // Red
		pixel[3] = 0xFF;                 // Alpha (opaque)
	}
}

// ==================== 清屏 ====================
// color: 0xRRGGBB (Red, Green, Blue), 自动设 Alpha 为 0xFF
void virtio_gpu_clear_screen(uint32_t color)
{
	if(gpu_framebuffer) {
		uint8_t r = (color >> 16) & 0xFF;
		uint8_t g = (color >> 8) & 0xFF;
		uint8_t b = color & 0xFF;
		for(uint32_t i = 0; i < gpu_width * gpu_height; i++) {
			uint8_t* pixel = (uint8_t*)&gpu_framebuffer[i];
			pixel[0] = b;
			pixel[1] = g;
			pixel[2] = r;
			pixel[3] = 0xFF;
		}
	}
}

// ==================== 初始化 virtio-gpu 设备 ====================
void virtio_gpu_init(void) {
    if(find_virtio_gpu_device() != 0) {
        return;
    }
    
    uint32_t magic = VIRTIO_REG(VIRTIO_MMIO_MAGIC_VALUE);
    if(magic != 0x74726976) {
        uart_puts("virtio-gpu: bad magic value\n");
        return;
    }
    
    uint32_t dev_id = VIRTIO_REG(VIRTIO_MMIO_DEVICE_ID);
    if(dev_id != VIRTIO_DEVICE_ID_GPU) {
        uart_puts("virtio-gpu: not a GPU device\n");
        return;
    }
    
    uart_puts("virtio-gpu: device found\n"); 
    
    // 1. 复位+基础状态
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = 0;
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE;
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER;
    
    // 2. 读取设备全部特性（低 + 高）
    VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES_SEL) = 0;
    uint32_t dev_feat_low = VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES);
    
    VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES_SEL) = 1;
    uint32_t dev_feat_high = VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES);
    
    uart_puts("virtio-gpu dev feat low: 0x");
    uart_put_hex(dev_feat_low);
    uart_puts("\n");
    uart_puts("virtio-gpu dev feat high: 0x");
    uart_put_hex(dev_feat_high);
    uart_puts("\n");
    
    #define VIRTIO_F_VERSION_1 32
    if(!(dev_feat_high & (1U << (VIRTIO_F_VERSION_1 - 32)))) {
        uart_puts("virtio-gpu: no VERSION_1\n");
        return;
    }
    
    // 写驱动特性（低 + 高）
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES_SEL) = 0;
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES) = 0;  // VERSION_1 隐含 ANY_LAYOUT
    
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES_SEL) = 1;
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES) = (1U << (VIRTIO_F_VERSION_1 - 32)); 
    
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER | VIRTIO_CONFIG_S_FEATURES_OK;
    if(!(VIRTIO_REG(VIRTIO_MMIO_STATUS) & VIRTIO_CONFIG_S_FEATURES_OK)) {
        uart_puts("virtio-gpu: features not accepted!\n");
        return; 
    }
    
    // 分配队列结构
    desc = (struct virtq_desc*)my_malloc(QUEUE_SIZE * sizeof(struct virtq_desc));
    avail = (struct virtq_avail*)my_malloc(4 + QUEUE_SIZE * 2);
    used = (struct virtq_used*)my_malloc(4 + QUEUE_SIZE * 8);
    
    if(!desc || !avail || !used) {
        uart_puts("virtio-gpu: out of memory!\n");
        return;
    }
    
    for(int i = 0; i < QUEUE_SIZE; i++) {
        free_desc[i] = 1;
    }
    
    avail->flags = 0;
    avail->idx = 0;
    used_idx = 0;
    
    // 配置控制队列 (queue 0)
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_SEL) = 0;
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_NUM) = QUEUE_SIZE;
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_DESC_LOW) = (uint32_t)(uint64_t)desc;
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_DESC_HIGH) = (uint32_t)((uint64_t)desc >> 32);
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_AVAIL_LOW) = (uint32_t)(uint64_t)avail;
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_AVAIL_HIGH) = (uint32_t)((uint64_t)avail >> 32);
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_USED_LOW) = (uint32_t)(uint64_t)used;
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_USED_HIGH) = (uint32_t)((uint64_t)used >> 32);
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_READY) = 1;
    
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER | VIRTIO_CONFIG_S_FEATURES_OK | VIRTIO_CONFIG_S_DRIVER_OK;
    uart_puts("virtio-gpu: initialized successfully (step 1 done)\n");
    
    // ==================== 第二步：获取显示信息 ====================
    if(get_display_info() != 0) {
        return;
    }
    
    // ==================== 第三步：分配帧缓冲区 ====================
    uint32_t fb_size = gpu_width * gpu_height * 4;
    uart_puts("virtio-gpu: framebuffer size ");
    uart_put_dec(fb_size);
    uart_puts(" bytes\n");
    gpu_framebuffer = (uint32_t*)my_malloc(fb_size);
    if(!gpu_framebuffer) {
        uart_puts("virtio-gpu: failed to allocate framebuffer!\n");
        return;
    }
    uart_puts("virtio-gpu: framebuffer allocated at 0x");
    uart_put_hex((uint64_t)gpu_framebuffer);
    uart_puts("\n");
    
    // ==================== 第四步：创建 2D 资源 ====================
    if(create_resource_2d(1, gpu_width, gpu_height) != 0) {
        return;
    }
    
    // ==================== 第五步：Attach Backing ====================
    if(attach_backing(1, gpu_framebuffer, fb_size) != 0) {
        return;
    }
    
    // ==================== 第六步：设置 Scanout ====================
    if(set_scanout(0, 1, gpu_width, gpu_height) != 0) {
        uart_puts("virtio-gpu: set_scanout failed, trying alternative...\n");
    } else {
        uart_puts("virtio-gpu: set_scanout succeeded!\n");
    }
    
    uart_puts("virtio-gpu: ALL STEPS DONE! Ready to draw!\n");
    
    // 测试：清屏为红色，然后刷新 (格式: 0xRRGGBB)
	virtio_gpu_clear_screen(0xFF0000); // R=0xFF, G=0x00, B=0x00
	virtio_gpu_flush();
	uart_puts("virtio-gpu: screen flushed! Should see red!\n");
}
