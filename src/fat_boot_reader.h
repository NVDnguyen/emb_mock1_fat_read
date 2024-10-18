#ifndef FAT_BOOT_READER_H
#define FAT_BOOT_READER_H

#include <stdint.h>
/*******************************************************************************
* Definitions
******************************************************************************/
#define BOOTSTRAP_SIZE 3
#define MANUFACTURER_DESC_SIZE 8
#define VOLUME_LABEL_SIZE 11
#define FILESYSTEM_IDENTIFIER_SIZE 8
#define BOOTSTRAP_REMAINDER_SIZE 0x1C0
/*******************************************************************************
* Struct Definitions
******************************************************************************/
typedef struct BootBlock {
    uint8_t bootstrap[BOOTSTRAP_SIZE];
    uint8_t manufacturer_desc[MANUFACTURER_DESC_SIZE]; 
    uint16_t bytes_per_block;
    uint8_t blocks_per_allocation_unit;
    uint16_t reserved_blocks;
    uint8_t num_fat;
    uint16_t root_dir_entries;
    uint16_t total_blocks;
    uint8_t media_descriptor;
    uint16_t blocks_per_fat;
    uint16_t blocks_per_track;
    uint16_t num_heads;
    uint32_t hidden_blocks;
    uint32_t total_blocks_extended;
    uint8_t physical_drive_number;
    uint8_t extended_boot_record_signature;
    uint32_t volume_serial_number;
    char volume_label[VOLUME_LABEL_SIZE];
    char filesystem_identifier[FILESYSTEM_IDENTIFIER_SIZE];
    uint8_t bootstrap_remainder[BOOTSTRAP_REMAINDER_SIZE];
    uint16_t boot_signature;
} BootBlock;
/*******************************************************************************
* Status code
******************************************************************************/
typedef enum {
    ERROR,
    OK
} status_t;
/*******************************************************************************
* This function 
******************************************************************************/
status_t readBootBlock(); 

#endif /* FAT_BOOT_READER_H */
