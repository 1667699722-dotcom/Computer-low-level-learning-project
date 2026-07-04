// ==================== 基本类型定义 ====================
typedef unsigned long      uint64_t;   // 64位无符号整数
typedef unsigned int       uint32_t;   // 32位无符号整数
typedef unsigned short     uint16_t;   // 16位无符号整数
typedef unsigned char      uint8_t;    // 8位无符号整数

// ==================== Virtio GPU 设备ID ====================
#define VIRTIO_DEVICE_ID_GPU            16      // virtio-gpu 设备 ID

// ==================== Virtio GPU 命令类型 ====================
#define VIRTIO_GPU_CMD_GET_DISPLAY_INFO 0x0100
#define VIRTIO_GPU_CMD_RESOURCE_CREATE_2D 0x0101
#define VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING 0x0106
#define VIRTIO_GPU_CMD_SET_SCANOUT 0x0103
#define VIRTIO_GPU_CMD_RESOURCE_FLUSH 0x0104
#define VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D 0x0105

// ==================== Virtio GPU 响应类型 ====================
#define VIRTIO_GPU_RESP_OK_NODATA 0x1100
#define VIRTIO_GPU_RESP_OK_DISPLAY_INFO 0x1101

// ==================== Virtio GPU 像素格式 ====================
#define VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM 1

// ==================== Virtio GPU 控制头 ====================
struct virtio_gpu_ctrl_hdr {
	uint32_t type;
	uint32_t flags;
	uint64_t fence_id;
	uint32_t ctx_id;
	uint8_t ring_idx;
	uint8_t padding[3];
};

// ==================== Virtio GPU 矩形 ====================
struct virtio_gpu_rect {
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
};

// ==================== Virtio GPU 内存 entry ====================
struct virtio_gpu_mem_entry {
	uint64_t addr;
	uint32_t length;
	uint32_t padding;
};

// ==================== Virtio GPU 显示信息 ====================
#define VIRTIO_GPU_MAX_SCANOUTS 16
struct virtio_gpu_display_one {
	struct virtio_gpu_rect r;
	uint32_t enabled;
	uint32_t flags;
};

struct virtio_gpu_resp_display_info {
	struct virtio_gpu_ctrl_hdr hdr;
	struct virtio_gpu_display_one pmodes[VIRTIO_GPU_MAX_SCANOUTS];
};

// ==================== Virtio GPU 资源创建 2D ====================
struct virtio_gpu_resource_create_2d {
	struct virtio_gpu_ctrl_hdr hdr;
	uint32_t resource_id;
	uint32_t format;
	uint32_t width;
	uint32_t height;
};

// ==================== Virtio GPU 资源 attach backing ====================
struct virtio_gpu_resource_attach_backing {
	struct virtio_gpu_ctrl_hdr hdr;
	uint32_t resource_id;
	uint32_t nr_entries;
};

// ==================== Virtio GPU 设置 scanout ====================
struct virtio_gpu_set_scanout {
	struct virtio_gpu_ctrl_hdr hdr;
	struct virtio_gpu_rect r;
	uint32_t scanout_id;
	uint32_t resource_id;
};

// ==================== Virtio GPU 资源 flush ====================
struct virtio_gpu_resource_flush {
	struct virtio_gpu_ctrl_hdr hdr;
	struct virtio_gpu_rect r;
	uint32_t resource_id;
	uint32_t padding;
};

// ==================== Virtio GPU transfer to host 2D ====================
struct virtio_gpu_transfer_to_host_2d {
	struct virtio_gpu_ctrl_hdr hdr;
	struct virtio_gpu_rect r;
	uint64_t offset;
	uint32_t resource_id;
	uint32_t padding;
};

// ==================== 全局状态 ====================
extern uint32_t gpu_width;
extern uint32_t gpu_height;
extern uint32_t* gpu_framebuffer;

// ==================== 函数声明 ====================
void virtio_gpu_init(void);
void virtio_gpu_draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void virtio_gpu_clear_screen(uint32_t color);
void virtio_gpu_flush(void);
