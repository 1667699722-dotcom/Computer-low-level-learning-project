// ==================== 基本类型定义 ====================
typedef unsigned long      uint64_t;   // 64位无符号整数
typedef unsigned int       uint32_t;   // 32位无符号整数
typedef unsigned short     uint16_t;   // 16位无符号整数
typedef unsigned char      uint8_t;    // 8位无符号整数

// ==================== Virtio MMIO 寄存器偏移 ====================
#define VIRTIO_MMIO_MAGIC_VALUE         0x000   // 魔法值，应该是 0x74726976
#define VIRTIO_MMIO_VERSION             0x004   // 版本号，QEMU virtio 版本 2
#define VIRTIO_MMIO_DEVICE_ID           0x008   // 设备类型 ID，块设备是 2
#define VIRTIO_MMIO_VENDOR_ID           0x00c   // 厂商 ID，应该是 0x554d4551
#define VIRTIO_MMIO_DEVICE_FEATURES     0x010   // 设备功能位（低32位）
#define VIRTIO_MMIO_DEVICE_FEATURES_SEL 0x014   // 设备功能位选择，用于访问高32位
#define VIRTIO_MMIO_DRIVER_FEATURES     0x020   // 驱动功能位（低32位）
#define VIRTIO_MMIO_DRIVER_FEATURES_SEL 0x024   // 驱动功能位选择
#define VIRTIO_MMIO_QUEUE_SEL           0x030   // 选择要配置的队列
#define VIRTIO_MMIO_QUEUE_NUM_MAX       0x034   // 队列的最大描述符数量
#define VIRTIO_MMIO_QUEUE_NUM           0x038   // 设置队列的描述符数量
#define VIRTIO_MMIO_QUEUE_READY         0x044   // 标记队列准备就绪
#define VIRTIO_MMIO_QUEUE_NOTIFY        0x050   // 通知设备有新的请求
#define VIRTIO_MMIO_INTERRUPT_STATUS    0x060   // 中断状态
#define VIRTIO_MMIO_INTERRUPT_ACK       0x064   // 中断确认
#define VIRTIO_MMIO_STATUS              0x070   // 设备状态寄存器
#define VIRTIO_MMIO_QUEUE_DESC_LOW      0x080   // 描述符表的物理地址（低32位）
#define VIRTIO_MMIO_QUEUE_DESC_HIGH     0x084   // 描述符表的物理地址（高32位）
#define VIRTIO_MMIO_QUEUE_AVAIL_LOW     0x090   // Available Ring 的物理地址（低32位）
#define VIRTIO_MMIO_QUEUE_AVAIL_HIGH    0x094   // Available Ring 的物理地址（高32位）
#define VIRTIO_MMIO_QUEUE_USED_LOW      0x0a0   // Used Ring 的物理地址（低32位）
#define VIRTIO_MMIO_QUEUE_USED_HIGH     0x0a4   // Used Ring 的物理地址（高32位）
#define VIRTIO_MMIO_CONFIG_GENERATION   0x0fc   // 配置生成号
#define VIRTIO_MMIO_CONFIG              0x100   // 设备特定配置的基地址

// ==================== 状态位 ====================
#define VIRTIO_CONFIG_S_ACKNOWLEDGE     1       // 驱动发现设备
#define VIRTIO_CONFIG_S_DRIVER          2       // 驱动知道如何驱动设备
#define VIRTIO_CONFIG_S_DRIVER_OK       4       // 驱动已准备好
#define VIRTIO_CONFIG_S_FEATURES_OK     8       // 功能协商完成
#define VIRTIO_CONFIG_S_DEVICE_NEEDS_RESET 64   // 设备需要重置
#define VIRTIO_CONFIG_S_FAILED          128     // 驱动出错

// ==================== 设备 ID ====================
#define VIRTIO_DEVICE_ID_BLOCK          2       // virtio-blk 块设备 ID

// ==================== Virtio Blk 功能位 ====================
#define VIRTIO_BLK_F_RO                 5       // 只读设备
#define VIRTIO_BLK_F_SCSI               7       // 支持 SCSI 命令
#define VIRTIO_BLK_F_CONFIG_WCE         11      // 支持写回缓存配置

// ==================== Virtio Blk 请求类型 ====================
#define VIRTIO_BLK_T_IN                 0       // 从设备读取
#define VIRTIO_BLK_T_OUT                1       // 写入设备

// ==================== Virtio Blk 请求状态 ====================
#define VIRTIO_BLK_S_OK                 0       // 请求成功
#define VIRTIO_BLK_S_IOERR              1       // IO 错误
#define VIRTIO_BLK_S_UNSUPP             2       // 不支持的请求

// ==================== QEMU AArch64 virt 平台 MMIO 基地址 ====================
//#define VIRTIO_MMIO_BASE                0x0a003e00  // virtio MMIO 设备的起始地址
#define VIRTIO_MMIO_SIZE                0x200       // 每个设备占 0x512 字节
// virtio-blk 通常是第二个设备（第一个是 virtio-net）
//#define VIRTIO_BLK_MMIO_BASE            (VIRTIO_MMIO_BASE + VIRTIO_MMIO_SIZE)






// ==================== 初始化 virtio-blk 设备 ====================
void virtio_blk_init(void);
int virtio_blk_read(unsigned long  sector, void* buffer);
int virtio_blk_write(uint64_t sector, const void* buffer);