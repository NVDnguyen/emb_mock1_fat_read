#ifndef FAT_BOOT_READER_H
#define FAT_BOOT_READER_H

#include <stdint.h>
#include "status_code.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOOTSTRAP_SIZE 3
#define MANUFACTURER_DESC_SIZE 8
#define VOLUME_LABEL_SIZE 11
#define SERIAL_NUMBER_SIZE 4
#define FILESYSTEM_IDENTIFIER_SIZE 8
#define BOOTSTRAP_REMAINDER_SIZE 448


/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/

typedef struct
{
    uint8_t bootstrap[BOOTSTRAP_SIZE];
    uint8_t manufacturer_desc[MANUFACTURER_DESC_SIZE];

    uint8_t bytes_per_block1;
    uint8_t bytes_per_block2;

    uint8_t blocks_per_allocation_unit; 
    uint16_t reserved_blocks; /*0x0E*/
    uint8_t num_fat;

    uint8_t root_dir_entries1;
    uint8_t root_dir_entries2;

    uint8_t total_blocks1;
    uint8_t total_blocks2;

    uint8_t media_descriptor;
    uint16_t blocks_per_fat;
    uint16_t blocks_per_track;
    uint16_t num_heads;
    uint32_t hidden_blocks;
    uint32_t total_blocks_per_entry;
    uint16_t physical_drive_number;
    uint8_t extended_boot_record_signature;
    uint8_t volume_serial_number[SERIAL_NUMBER_SIZE];    
    uint8_t volume_label[VOLUME_LABEL_SIZE];
    uint8_t filesystem_identifier[FILESYSTEM_IDENTIFIER_SIZE];
    uint8_t bootstrap_remainder[BOOTSTRAP_REMAINDER_SIZE];
    
    uint8_t boot_signature1;
    uint8_t boot_signature2;
}  __attribute__((packed)) BootBlockReader;

typedef struct
{
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
    uint32_t total_blocks_per_entry;
    uint16_t physical_drive_number;
    uint8_t extended_boot_record_signature;
    uint8_t volume_serial_number[SERIAL_NUMBER_SIZE];    
    uint8_t volume_label[VOLUME_LABEL_SIZE];
    uint8_t filesystem_identifier[FILESYSTEM_IDENTIFIER_SIZE];
    uint8_t bootstrap_remainder[BOOTSTRAP_REMAINDER_SIZE];
    uint16_t boot_signature;
}  BootBlock;


/*******************************************************************************
 * Global var
 ******************************************************************************/
BootBlock bootBlock;

/*******************************************************************************
 * This function
 ******************************************************************************/
status_t readBootBlock();


#endif /* FAT_BOOT_READER_H */
