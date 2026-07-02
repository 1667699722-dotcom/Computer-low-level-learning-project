#include "include/uart.h"
#include "include/lib.h"
#include "include/virtio.h"

// ==================== Virtqueue 描述符结构 ====================
// 每个描述符指向一个内存缓冲区
struct virtq_desc {
    uint64_t addr;    // 缓冲区的物理地址
    uint32_t len;     // 缓冲区的长度
    uint16_t flags;   // 标志位
    uint16_t next;    // 下一个描述符的索引（如果设置了 NEXT 标志）
};

#define VRING_DESC_F_NEXT               1   // 描述符链中的下一个
#define VRING_DESC_F_WRITE              2   // 设备可以写入此缓冲区
#define VRING_DESC_F_INDIRECT           4   // 间接描述符

// ==================== Available Ring ====================
// 驱动告诉设备哪些描述符链可用
struct virtq_avail {
    uint16_t flags;   // 标志位
    uint16_t idx;     // 下一个可用的 ring 索引
    uint16_t ring[];  // 描述符链的头索引
};

#define VRING_AVAIL_F_NO_INTERRUPT      1   // 设备处理完后不发送中断

// ==================== Used Ring 元素 ====================
// 设备告诉驱动哪些描述符链已处理完
struct virtq_used_elem {
    uint32_t id;      // 描述符链的头索引
    uint32_t len;     // 写入的总长度
};

// ==================== Used Ring ====================
struct virtq_used {
    uint16_t flags;   // 标志位
    uint16_t idx;     // 下一个可用的 ring 索引
    struct virtq_used_elem ring[];  // 已处理的描述符链
};

#define VRING_USED_F_NO_NOTIFY          1   // 不通知设备

// ==================== Virtio Blk 请求头 ====================
// 每个 IO 请求的头部结构
struct virtio_blk_req {
    uint32_t type;     // 请求类型：IN 或 OUT
    uint32_t reserved; // 保留字段
    uint64_t sector;   // 要读写的扇区号（以 512 字节为单位）
};

// ==================== 驱动全局变量 ====================
#define QUEUE_SIZE 8                       // 队列的描述符数量
static struct virtq_desc* desc;        // 描述符表指针
static struct virtq_avail* avail;      // Available Ring 指针
static struct virtq_used* used;        // Used Ring 指针
static uint16_t used_idx;              // 上一次处理的 Used Ring 索引
static struct virtio_blk_req* reqs;    // 请求头数组
static uint8_t* statuses;              // 状态字节数组
static uint8_t free_desc[QUEUE_SIZE];  //标记空闲描述符
// ==================== MMIO 寄存器访问宏 ====================
// 访问 virtio-blk 设备的 MMIO 寄存器
static uint64_t virtio_blk_mmio_base = 0;  
#define VIRTIO_REG(reg) (*(volatile uint32_t*)(virtio_blk_mmio_base + (reg)))

// ==================== 查找 virtio-blk 设备 ====================
static int find_virtio_blk_device() {
    // QEMU virt 平台 virtio 设备从 0x0a000000 开始，每个占 0x200
    for (uint64_t base = 0x0a000000; base < 0x0a010000; base += 0x200) {
        // 检查魔法值
        volatile uint32_t* magic = (volatile uint32_t*)base;
        if (*magic != 0x74726976) continue;
        
        // 检查设备 ID
        volatile uint32_t* dev_id = (volatile uint32_t*)(base + VIRTIO_MMIO_DEVICE_ID);
        if (*dev_id == VIRTIO_DEVICE_ID_BLOCK) {
            //uart_puts("virtio-blk: found at 0x");
            //uart_put_hex(base);
            //uart_puts("\n");
            virtio_blk_mmio_base = base;
            return 0;
        }
    }
    //uart_puts("virtio-blk: device not found\n");
    return -1;
}
// ==================== 函数实现 ====================
static int alloc_desc()
{
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        if(free_desc[i])
        {
            free_desc[i]=0;
            return i;
        }
    }
    return -1;
}
// ==================== 释放描述符链 ====================
static void free_desc_chain(int idx)
{
    while(idx!=-1)
    {
        int next=desc[idx].next;
        free_desc[idx]=1;
        if(desc[idx].flags & VRING_DESC_F_NEXT)
        {
            idx=next;
        }
        else 
        {
            break;
        }
    }
}
// ==================== 初始化 virtio-blk 设备 ====================
void virtio_blk_init(void)
{   
    if (find_virtio_blk_device() != 0) {
        return;
    }
    uint32_t magic = VIRTIO_REG(VIRTIO_MMIO_MAGIC_VALUE);
    if (magic != 0x74726976) 
    {
        uart_puts("virtio-blk: bad magic value\n");
        return;
    }

    uint32_t dev_id = VIRTIO_REG(VIRTIO_MMIO_DEVICE_ID);
    if (dev_id != VIRTIO_DEVICE_ID_BLOCK) {
        uart_puts("virtio-blk: not a block device\n");
        return;
    }

    uart_puts("virtio-blk: device found\n"); 
    
    // 1. 复位+基础状态
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = 0;
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE;
    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER;

    // 2. 读取设备全部特性（低 + 高）
    VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES_SEL) = 0;  // 选低 32 位
    uint32_t dev_feat_low = VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES);

    VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES_SEL) = 1;  // 选高 32 位
    uint32_t dev_feat_high = VIRTIO_REG(VIRTIO_MMIO_DEVICE_FEATURES);

    uart_puts("dev feat low: 0x"); uart_put_hex(dev_feat_low); uart_puts("\n");
    uart_puts("dev feat high: 0x"); uart_put_hex(dev_feat_high); uart_puts("\n");

    #define VIRTIO_F_VERSION_1 32
    // 检查 VERSION_1 在高 32 位的 bit 0
    if (!(dev_feat_high & (1U << (VIRTIO_F_VERSION_1 - 32)))) {
        uart_puts("virtio-blk: no VERSION_1\n");
        return;
    }

    // 写驱动特性（低 + 高）
    #define VIRTIO_F_ANY_LAYOUT 27
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES_SEL) = 0;
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES) = (1U << VIRTIO_F_ANY_LAYOUT); // 低 32 位

    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES_SEL) = 1;
    VIRTIO_REG(VIRTIO_MMIO_DRIVER_FEATURES) = (1U << (VIRTIO_F_VERSION_1 - 32)); 

    VIRTIO_REG(VIRTIO_MMIO_STATUS) = VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER | VIRTIO_CONFIG_S_FEATURES_OK;
    if (!(VIRTIO_REG(VIRTIO_MMIO_STATUS) & VIRTIO_CONFIG_S_FEATURES_OK)) 
    {
        uart_puts("virtio-blk: features not accepted\n");
        return; 
    }
    desc = (struct virtq_desc*)my_malloc(QUEUE_SIZE * sizeof(struct virtq_desc));
    avail=(struct virtq_avail*)my_malloc(4+QUEUE_SIZE*2);
    used = (struct virtq_used*)my_malloc(4 + QUEUE_SIZE * 8);
    reqs = (struct virtio_blk_req*)my_malloc(QUEUE_SIZE * sizeof(struct virtio_blk_req));
    statuses = (uint8_t*)my_malloc(QUEUE_SIZE);

    if (!desc || !avail || !used || !reqs || !statuses) {
        uart_puts("virtio-blk: out of memory\n");
        return;
    }
    
    for (int i = 0; i < QUEUE_SIZE; i++) {
        free_desc[i] = 1;
    }

    avail->flags = 0;
    avail->idx = 0;
    used_idx = 0;

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
    uart_puts("virtio-blk: initialized successfully\n");
}

int virtio_blk_read(uint64_t sector, void* buffer) {
    // 分配 3 个描述符
    int idx0 = alloc_desc();
    int idx1 = alloc_desc();
    int idx2 = alloc_desc();

    if (idx0 == -1 || idx1 == -1 || idx2 == -1) {
        uart_puts("virtio-blk: no free descriptors\n");
        free_desc_chain(idx0);
        free_desc_chain(idx1);
        free_desc_chain(idx2);
        return -1;
    }

    // 描述符 0: 请求头
    reqs[idx0].type = VIRTIO_BLK_T_IN;
    reqs[idx0].reserved = 0;
    reqs[idx0].sector = sector;
    desc[idx0].addr = (uint64_t)&reqs[idx0];
    desc[idx0].len = sizeof(struct virtio_blk_req);
    desc[idx0].flags = VRING_DESC_F_NEXT;
    desc[idx0].next = idx1;

    // 描述符 1: 数据缓冲区 (设备写)
    desc[idx1].addr = (uint64_t)buffer;
    desc[idx1].len = 512;
    desc[idx1].flags = VRING_DESC_F_NEXT | VRING_DESC_F_WRITE;
    desc[idx1].next = idx2;

    // 描述符 2: 状态字节 (设备写)
    statuses[idx0] = 0xFF;
    desc[idx2].addr = (uint64_t)&statuses[idx0];
    desc[idx2].len = 1;
    desc[idx2].flags = VRING_DESC_F_WRITE;
    desc[idx2].next = 0;

    // 添加到 Available Ring
    avail->ring[avail->idx % QUEUE_SIZE] = idx0;
    avail->idx++;

    // 通知设备
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_NOTIFY) = 0;

    // 等待设备处理 (轮询)
    while (used_idx == used->idx) {
        // 空转
    }

    // 检查状态
    int res = (statuses[idx0] == VIRTIO_BLK_S_OK) ? 0 : -1;

    // 释放描述符
    free_desc_chain(idx0);
    used_idx++;

    return res;
}

int virtio_blk_write(uint64_t sector, const void* buffer) {
    // 分配 3 个描述符
    int idx0 = alloc_desc();
    int idx1 = alloc_desc();
    int idx2 = alloc_desc();

    if (idx0 == -1 || idx1 == -1 || idx2 == -1) {
        uart_puts("virtio-blk: no free descriptors\n");
        free_desc_chain(idx0);
        free_desc_chain(idx1);
        free_desc_chain(idx2);
        return -1;
    }

    // 描述符 0: 请求头
    reqs[idx0].type = VIRTIO_BLK_T_OUT;
    reqs[idx0].reserved = 0;
    reqs[idx0].sector = sector;
    desc[idx0].addr = (uint64_t)&reqs[idx0];
    desc[idx0].len = sizeof(struct virtio_blk_req);
    desc[idx0].flags = VRING_DESC_F_NEXT;
    desc[idx0].next = idx1;

    // 描述符 1: 数据缓冲区 (设备读)
    desc[idx1].addr = (uint64_t)buffer;
    desc[idx1].len = 512;
    desc[idx1].flags = VRING_DESC_F_NEXT;
    desc[idx1].next = idx2;

    // 描述符 2: 状态字节 (设备写)
    statuses[idx0] = 0xFF;
    desc[idx2].addr = (uint64_t)&statuses[idx0];
    desc[idx2].len = 1;
    desc[idx2].flags = VRING_DESC_F_WRITE;
    desc[idx2].next = 0;

    // 添加到 Available Ring
    avail->ring[avail->idx % QUEUE_SIZE] = idx0;
    avail->idx++;

    // 通知设备
    VIRTIO_REG(VIRTIO_MMIO_QUEUE_NOTIFY) = 0;

    // 等待设备处理 (轮询)
    while (used_idx == used->idx) {
        // 空转
    }

    // 检查状态
    int res = (statuses[idx0] == VIRTIO_BLK_S_OK) ? 0 : -1;

    // 释放描述符
    free_desc_chain(idx0);
    used_idx++;

    return res;
}

