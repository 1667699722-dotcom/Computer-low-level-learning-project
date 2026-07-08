#include "include/fs.h"
#include "include/lib.h"
#include "include/uart.h"
#include "include/virtio.h"

static superblock_t sb;
static uint8_t bitmap[FS_BLOCK_SUM];
static dir_entry_t root_dir[FS_MAX_FILES];

static int fs_initialized = 0;
//找到空闲块
static int find_free_block(void)
{
    for(int i=0;i<FS_BLOCK_SIZE;i++)
    {
        if(bitmap[i]!=0xFF)
        {
            for(int j=0;j<8;j++)
            {
                if(!(bitmap[i]&(1<<j)))
                {
                    return 8*i+j;
                }
            }
        }
    }
    return -1;
}
//  标记块使用
static void mark_block_used(int block)
{
    int byte=block/8;
    int bit=block%8;
    bitmap[byte]|=(1<<bit);
}
//  标记块空闲
static void mark_block_free(int block) {
    int byte = block / 8;
    int bit = block % 8;
    bitmap[byte] &= ~(1 << bit);
}

//在目录中找到同文件名的文件目录序号
static int find_file(const char *filename)
{
    for(int i=0;i<FS_MAX_FILES;i++)
    {
        if(root_dir[i].in_use && my_strcmp(root_dir[i].name, filename)==0)
        {
            return i;
        }
    }
    return -1;
}

static int find_free_dir_entry(void)
{
    for(int i=0;i<FS_MAX_FILES;i++)
    {
        if(!root_dir[i].in_use)
        {return i;}
    }
    return -1;
}

static void write_superblock(void)
{
    virtio_blk_write(0, &sb);
}

static void read_superblock(void)
{
    virtio_blk_read(0, &sb);
}

static void write_bitmap(void) {
    virtio_blk_write(1, bitmap);
}

static void read_bitmap(void) {
    virtio_blk_read(1, bitmap);
}

static void write_root_dir(void) {
    uint8_t buf[FS_BLOCK_SIZE];
    my_memset(buf, 0, FS_BLOCK_SIZE);
    my_memcpy(buf, root_dir, sizeof(root_dir));
    virtio_blk_write(2, buf);
}

static void read_root_dir(void) {
    uint8_t buf[FS_BLOCK_SIZE];
    virtio_blk_read(2, buf);
    my_memcpy(root_dir, buf, sizeof(root_dir));
}

void fs_format(void)
{
    //uart_puts("Formatting filesystem... \n");

    //uart_puts("fs_format: setting superblock...\n");
    sb.magic=FS_MAGIC;
    sb.total_blocks=2048;
    sb.bitmap_blocks=1;
    sb.data_start=3;
    sb.root_dir_block=2;

    //uart_puts("fs_format: clearing bitmap...\n");
    my_memset(bitmap, 0, FS_BLOCK_SUM);
    //uart_puts("fs_format: marking blocks used...\n");
    mark_block_used(0);
    mark_block_used(1);
    mark_block_used(2);

    //uart_puts("fs_format: clearing root_dir...\n");
    my_memset(root_dir, 0, sizeof(root_dir));

    //uart_puts("fs_format: write_superblock...\n");
    write_superblock();
    //uart_puts("fs_format: write_bitmap...\n");
    write_bitmap();
    //uart_puts("fs_format: write_root_dir...\n");
    write_root_dir();

    //uart_puts("Filesystem formatted!\n");
    fs_initialized = 1;
}

void fs_init(void) {
    //uart_puts("Initializing filesystem...\n");
    
    read_superblock();
    
    if (sb.magic != FS_MAGIC) {
        uart_puts("No valid filesystem found, formatting...\n");
        fs_format();
    } else {
        read_bitmap();
        read_root_dir();
        fs_initialized = 1;
        //uart_puts("Filesystem mounted!\n");
    }
}

int fs_create(const char* filename)
{
    if(!fs_initialized)
    {
        uart_puts("Filesystem not initialized!\n");
        return -1;
    }

    if (my_strlen(filename) >= FS_FILENAME_LEN) {
        uart_puts("Filename too long!\n");
        return -1;
    }

    if (find_file(filename) >= 0) {
        uart_puts("File already exists!\n");
        return -1;
    }

    int dir_idx = find_free_dir_entry();
    if (dir_idx < 0) {
        uart_puts("No free directory entries!\n");
        return -1;
    }

    int data_block = find_free_block();
    if (data_block < 0) {
        uart_puts("No free data blocks!\n");
        return -1;
    }

    my_strncpy(root_dir[dir_idx].name, filename, FS_FILENAME_LEN - 1);
    root_dir[dir_idx].name[FS_FILENAME_LEN - 1] = '\0';
    root_dir[dir_idx].size = 0;
    root_dir[dir_idx].start_block = data_block;
    root_dir[dir_idx].in_use = 1;

    mark_block_used(data_block);

    write_root_dir();
    write_bitmap();

    uart_puts("File created: ");
    uart_puts(filename);
    uart_puts("\n");
    
    return dir_idx;
}

int fs_write(const char* filename,const void *data,uint32_t size)
{
    if(!fs_initialized)
    {
        uart_puts("Filesystem not initialized!\n");
        return -1;
    }

    int file_idx = find_file(filename);

    if (file_idx < 0) {
        uart_puts("File not found!\n");
        return -1;
    }

    if (size > FS_MAX_FILESIZE) {
        uart_puts("File too large!\n");
        return -1;
    }

    uint32_t blocks_needed = (size + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE;
    uint32_t current_block = root_dir[file_idx].start_block;

    const uint8_t *src = (const uint8_t *)data;
    uint32_t remaining = size;

    for(uint32_t i=0;i<blocks_needed && remaining>0;i++)
    {
        uint8_t block_buf[FS_BLOCK_SIZE];
        uint32_t copy_size = (remaining > FS_BLOCK_SIZE) ? FS_BLOCK_SIZE : remaining;

        my_memset(block_buf, 0, FS_BLOCK_SIZE);
        my_memcpy(block_buf, src, copy_size);
        virtio_blk_write(sb.data_start + current_block, block_buf);
        src += copy_size;
        remaining -= copy_size;
        if (remaining > 0) {
            int next_block = find_free_block();
            if (next_block < 0) {
                uart_puts("Out of space during write!\n");
                return -1;
            }
            mark_block_used(next_block);
            current_block = next_block;
        }

    }
    root_dir[file_idx].size = size;

    write_root_dir();
    write_bitmap();
    
    uart_puts("Wrote ");
    uart_put_dec(size);
    uart_puts(" bytes to ");
    uart_puts(filename);
    uart_puts("\n");
    
    return size;
}

int fs_read(const char* filename, void *data, uint32_t size)
{
    if(!fs_initialized)
    {
        uart_puts("Filesystem not initialized!\n");
        return -1;
    }

    int file_idx = find_file(filename);

    if (file_idx < 0) {
        uart_puts("File not found!\n");
        return -1;
    }

    uint32_t read_size = (size > root_dir[file_idx].size) ? root_dir[file_idx].size : size;
    uint32_t blocks_needed = (read_size + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE;
    uint32_t current_block = root_dir[file_idx].start_block;

    uint8_t *dst = (uint8_t *)data;
    uint32_t remaining = read_size;

    for(uint32_t i=0;i<blocks_needed && remaining>0;i++)
    {
        uint8_t block_buf[FS_BLOCK_SIZE];
        virtio_blk_read(sb.data_start + current_block, block_buf);

        uint32_t copy_size = (remaining > FS_BLOCK_SIZE) ? FS_BLOCK_SIZE : remaining;
        my_memcpy(dst, block_buf, copy_size);

        dst += copy_size;
        remaining -= copy_size;
        if (remaining > 0) {
            current_block++;
        }
    }

    uart_puts("Read ");
    uart_put_dec(read_size);
    uart_puts(" bytes from ");
    uart_puts(filename);
    uart_puts("\n");

    return read_size;
}

int fs_delete(const char* filename)
{
    if(!fs_initialized)
    {
        uart_puts("Filesystem not initialized!\n");
        return -1;
    }

    int file_idx = find_file(filename);

    if (file_idx < 0) {
        uart_puts("File not found!\n");
        return -1;
    }

    uint32_t blocks_to_free = (root_dir[file_idx].size + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE;
    uint32_t current_block = root_dir[file_idx].start_block;

    for(uint32_t i=0;i<blocks_to_free;i++)
    {
        mark_block_free(current_block);
        current_block++;
    }

    my_memset(&root_dir[file_idx], 0, sizeof(dir_entry_t));

    write_root_dir();
    write_bitmap();

    uart_puts("File deleted: ");
    uart_puts(filename);
    uart_puts("\n");

    return 0;
}

void fs_list(void)
{
    if(!fs_initialized)
    {
        uart_puts("Filesystem not initialized!\n");
        return;
    }

    uart_puts("=== File List ===\n");
    int count = 0;

    for(int i=0;i<FS_MAX_FILES;i++)
    {
        if(root_dir[i].in_use)
        {
            uart_puts("- ");
            uart_puts(root_dir[i].name);
            uart_puts(" (");
            uart_put_dec(root_dir[i].size);
            uart_puts(" bytes)\n");
            count++;
        }
    }

    if(count == 0)
    {
        uart_puts("No files\n");
    }
    uart_puts("================\n");
}














