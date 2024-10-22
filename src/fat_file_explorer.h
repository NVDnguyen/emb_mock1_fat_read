#ifndef _FAT_FILE_EXPLORER_
#define _FAT_FILE_EXPLORER_
#include <stdint.h>
#include "fat_root_reader.h"
#include "fat_boot_reader.h"
typedef enum {
    FILE_ATTR_READ_ONLY    = 0x01, 
    FILE_ATTR_HIDDEN       = 0x02, 
    FILE_ATTR_SYSTEM       = 0x04, 
    FILE_ATTR_VOLUME_LABEL = 0x08,
    FILE_ATTR_DIRECTORY    = 0x10, 
    FILE_ATTR_ARCHIVE      = 0x20, 
    FILE_ATTR_UNUSED_1     = 0x40, 
    FILE_ATTR_UNUSED_2     = 0x80 
} FileAttributes;


typedef struct FileTime {
    uint8_t seconds : 5;     // 0-29 (0-58)
    uint8_t minutes : 6;     // 0-59
    uint8_t hours : 5;       // 0-23
} __attribute__((packed)) FileTime; /*size = 16bit*/

typedef struct FileDate {
    uint8_t day : 5;         // 1-31
    uint8_t month : 4;       // 1-12
    uint8_t year : 7;        //  (0-119) 1980 to 2099
} __attribute__((packed)) FileDate; /*size = 16bit*/

uint16_t next_cluster(uint16_t startingCluster, uint16_t seekOffset, FILE *f);
void display_data(const BootBlock *boot, const DirectoryEntry *file);


#endif /*_FAT_FILE_EXPLORER_*/