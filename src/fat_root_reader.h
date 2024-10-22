#ifndef FAT_ROOT_READER_H
#define FAT_ROOT_READER_H
#include "status_code.h"
#include <stdint.h>
#pragma pack(push, 1)
typedef struct {
    char name[8];         // Tên tệp (8 ký tự)
    char ext[3];          // Phần mở rộng tệp (3 ký tự)
    uint8_t attr;         // Thuộc tính tệp/thư mục
    uint8_t reserved[10]; // Dự trữ
    uint16_t firstCluster;// Cluster đầu tiên của tệp/thư mục
    uint32_t fileSize;    // Kích thước tệp (nếu là thư mục thì bằng 0)
} DirectoryEntry;
#pragma pop()
status_t readRoot(DirectoryEntry *arr,uint16_t rootEntryCount,uint32_t rootDirByteOffset,int *len);

#endif