#ifndef FS_H
#define FS_H

#include "lib.h"
#include "uart.h"
#include "virtio.h"

#define FS_MAGIC 0x46535953  // "FSYS"
#define FS_BLOCK_SIZE 512
#define FS_BLOCK_SUM 2048
#define FS_MAX_FILES 8
#define FS_FILENAME_LEN 32
#define FS_MAX_FILESIZE (1024 * 1024)  // 1MB 

typedef struct {
    uint32_t magic;           // 魔数
    uint32_t total_blocks;    // 总块数
    uint32_t bitmap_blocks;   // 位图占用块数
    uint32_t data_start;      // 数据区起始块号
    uint32_t root_dir_block;  // 根目录块号
} superblock_t;

typedef struct{
    char name[FS_FILENAME_LEN];  //文件名
    uint32_t size;               //文件大小
    uint32_t start_block;//起始数据块
    uint8_t in_use;//是否使用中
} dir_entry_t;

// 文件系统初始化
void fs_init(void);
// 创建文件
int fs_create(const char *filename);
// 写入文件
int fs_write(const char *filename, const void *data, uint32_t size);
// 读取文件
int fs_read(const char *filename, void *data, uint32_t size);
// 删除文件
int fs_delete(const char *filename);
// 列出所有文件
void fs_list(void);
// 格式化文件系统
void fs_format(void);

#endif
