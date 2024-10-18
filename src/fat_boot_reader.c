#include <stdint.h>
#include <stdio.h>
#include <string.h> 
#include "fat_boot_reader.h"

static BootBlockReader bootBlockReader;

status_t readBootBlock() {
    status_t status = ERROR;
    FILE *f = fopen("data/floppy.img", "rb");
    
    if (f != NULL) {
        /*Read the boot block into bootBlockReader*/ 
        status = (fread(&bootBlockReader, sizeof(BootBlockReader), 1, f) != 1) ? READ_ERROR : OK;

        /* Check Boot block 'signature'*/
        if (status == OK) {
            status = (bootBlockReader.boot_signature2 != 0xAA) ? READ_ERROR : OK;
        }

        /*Transfer data from bootBlockReader to bootBlock*/
        if (status == OK) {
            // Copy fixed-size arrays
            memcpy(bootBlock.bootstrap, bootBlockReader.bootstrap, BOOTSTRAP_SIZE);
            memcpy(bootBlock.manufacturer_desc, bootBlockReader.manufacturer_desc, MANUFACTURER_DESC_SIZE);
            memcpy(bootBlock.volume_serial_number, bootBlockReader.volume_serial_number, SERIAL_NUMBER_SIZE);
            memcpy(bootBlock.volume_label, bootBlockReader.volume_label, VOLUME_LABEL_SIZE);
            memcpy(bootBlock.filesystem_identifier, bootBlockReader.filesystem_identifier, FILESYSTEM_IDENTIFIER_SIZE);
            memcpy(bootBlock.bootstrap_remainder, bootBlockReader.bootstrap_remainder, BOOTSTRAP_REMAINDER_SIZE);

            /*Combine 2*uint8 to uint16*/ 
            bootBlock.bytes_per_block = (bootBlockReader.bytes_per_block1 | (bootBlockReader.bytes_per_block2 << 8));
            bootBlock.root_dir_entries = (bootBlockReader.root_dir_entries1 | (bootBlockReader.root_dir_entries2 << 8));
            bootBlock.total_blocks = (bootBlockReader.total_blocks1 | (bootBlockReader.total_blocks2 << 8));
            bootBlock.boot_signature = (bootBlockReader.boot_signature1 | (bootBlockReader.boot_signature2 << 8));

            /*coppy*/
            bootBlock.blocks_per_allocation_unit = bootBlockReader.blocks_per_allocation_unit;
            bootBlock.reserved_blocks = bootBlockReader.reserved_blocks;
            bootBlock.num_fat = bootBlockReader.num_fat;
            bootBlock.media_descriptor = bootBlockReader.media_descriptor;
            bootBlock.blocks_per_fat = bootBlockReader.blocks_per_fat;
            bootBlock.blocks_per_track = bootBlockReader.blocks_per_track;
            bootBlock.num_heads = bootBlockReader.num_heads;
            bootBlock.hidden_blocks = bootBlockReader.hidden_blocks;
            bootBlock.total_blocks_per_entry = bootBlockReader.total_blocks_per_entry;
            bootBlock.physical_drive_number = bootBlockReader.physical_drive_number;
            bootBlock.extended_boot_record_signature = bootBlockReader.extended_boot_record_signature;
        }
        fclose(f);
    } else {
        status = FILE_NULL_ERROR;
    }

    return status;
}
