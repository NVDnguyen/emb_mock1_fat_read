#include <stdint.h>
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "fat_boot_reader.h"



status_t readBootBlock(BootBlock *bootBlock,FILE *f ) {
    BootBlockReader bootBlockReader;
    status_t status = ERROR;

    
    if (f != NULL) {
        /*Read the boot block into bootBlockReader*/ 
        status = (fread(&bootBlockReader, sizeof(BootBlockReader), 1, f) != 1) ? ERROR_READ : OK;

        /* Check Boot block 'signature'*/
        if (status == OK) {
            status = (bootBlockReader.boot_signature2 != 0xAA) ? ERROR_READ : OK;
        }

        /*Transfer data from bootBlockReader to bootBlock*/
        if (status == OK) {
            // Copy fixed-size arrays
            memcpy(bootBlock->bootstrap, bootBlockReader.bootstrap, BOOTSTRAP_SIZE);
            memcpy(bootBlock->manufacturer_desc, bootBlockReader.manufacturer_desc, MANUFACTURER_DESC_SIZE);
            memcpy(bootBlock->volume_serial_number, bootBlockReader.volume_serial_number, SERIAL_NUMBER_SIZE);
            memcpy(bootBlock->volume_label, bootBlockReader.volume_label, VOLUME_LABEL_SIZE);
            memcpy(bootBlock->filesystem_identifier, bootBlockReader.filesystem_identifier, FILESYSTEM_IDENTIFIER_SIZE);
            memcpy(bootBlock->bootstrap_remainder, bootBlockReader.bootstrap_remainder, BOOTSTRAP_REMAINDER_SIZE);

            /*Combine 2*uint8 to uint16*/ 
            bootBlock->bytes_per_block = (bootBlockReader.bytes_per_block1 | (bootBlockReader.bytes_per_block2 << 8));
            bootBlock->num_root_dir_entries = (bootBlockReader.root_dir_entries1 | (bootBlockReader.root_dir_entries2 << 8));
            bootBlock->total_blocks = (bootBlockReader.total_blocks1 | (bootBlockReader.total_blocks2 << 8));
            bootBlock->boot_signature = (bootBlockReader.boot_signature1 | (bootBlockReader.boot_signature2 << 8));

            bootBlock->blocks_per_allocation_unit = bootBlockReader.blocks_per_allocation_unit;
            bootBlock->reserved_blocks = bootBlockReader.reserved_blocks;
            bootBlock->num_fat = bootBlockReader.num_fat;
            bootBlock->media_descriptor = bootBlockReader.media_descriptor;
            bootBlock->blocks_per_fat = bootBlockReader.blocks_per_fat;
            bootBlock->blocks_per_track = bootBlockReader.blocks_per_track;
            bootBlock->num_heads = bootBlockReader.num_heads;
            bootBlock->hidden_blocks = bootBlockReader.hidden_blocks;
            bootBlock->total_blocks_per_entry = bootBlockReader.total_blocks_per_entry;
            bootBlock->physical_drive_number = bootBlockReader.physical_drive_number;
            bootBlock->extended_boot_record_signature = bootBlockReader.extended_boot_record_signature;
        }
    } else {
        status = ERROR_NULL_FILE;
    }

    return status;
}
void printBootBlock(const BootBlock *bootBlock) {

    if (bootBlock == NULL) {
        printf("BootBlock is NULL\n");
        return;
    }

    printf("BootBlock Contents (in Hexadecimal):\n");
    
    /*Print bootstrap*/ 
    printf("Bootstrap: ");
    for (size_t i = 0; i < BOOTSTRAP_SIZE; i++) {
        printf("%02X ", bootBlock->bootstrap[i]);
    }
    printf("\n");

   /*Print manufacturer description*/
    printf("Manufacturer Description: ");
    for (size_t i = 0; i < MANUFACTURER_DESC_SIZE; i++) {
        printf("%02X ", bootBlock->manufacturer_desc[i]);
    }
    printf("\n");

    /*Print other fields*/
    printf("Bytes Per Block: 0x%04X\n", bootBlock->bytes_per_block);
    printf("Blocks Per Allocation Unit: 0x%02X\n", bootBlock->blocks_per_allocation_unit);
    printf("Reserved Blocks: 0x%04X\n", bootBlock->reserved_blocks);
    printf("Number of FAT: 0x%02X\n", bootBlock->num_fat);
    printf("Number of Root Directory Entries: 0x%04X\n", bootBlock->num_root_dir_entries);
    printf("Total Blocks: 0x%04X\n", bootBlock->total_blocks);
    printf("Media Descriptor: 0x%02X\n", bootBlock->media_descriptor);
    printf("Blocks Per FAT: 0x%04X\n", bootBlock->blocks_per_fat);
    printf("Blocks Per Track: 0x%04X\n", bootBlock->blocks_per_track);
    printf("Number of Heads: 0x%04X\n", bootBlock->num_heads);
    printf("Hidden Blocks: 0x%08X\n", bootBlock->hidden_blocks);
    printf("Total Blocks Per Entry: 0x%08X\n", bootBlock->total_blocks_per_entry);
    printf("Physical Drive Number: 0x%04X\n", bootBlock->physical_drive_number);
    printf("Extended Boot Record Signature: 0x%02X\n", bootBlock->extended_boot_record_signature);

    /*Print volume serial number*/
    printf("Volume Serial Number: ");
    for (size_t i = 0; i < SERIAL_NUMBER_SIZE; i++) {
        printf("%02X ", bootBlock->volume_serial_number[i]);
    }
    printf("\n");

    /*Print volume label*/
    printf("Volume Label: ");
    for (size_t i = 0; i < VOLUME_LABEL_SIZE; i++) {
        printf("%02X ", bootBlock->volume_label[i]);
    }
    printf("\n");

    /*Print filesystem identifier*/
    printf("Filesystem Identifier: ");
    for (size_t i = 0; i < FILESYSTEM_IDENTIFIER_SIZE; i++) {
        printf("%02X ", bootBlock->filesystem_identifier[i]);
    }
    printf("\n");

    /*Print bootstrap remainder*/
    printf("Bootstrap Remainder: ");
    for (size_t i = 0; i < BOOTSTRAP_REMAINDER_SIZE; i++) {
        printf("%02X ", bootBlock->bootstrap_remainder[i]);
    }
    printf("\n");

    /*Print boot signature*/
    printf("Boot Signature: 0x%04X\n", bootBlock->boot_signature);
}

status_t verifyFATTable(BootBlock *bootBlock, FILE *f) {
    uint8_t numFAT = bootBlock->num_fat;
    uint32_t sizeOfFAT = bootBlock->blocks_per_fat * bootBlock->bytes_per_block;
    uint32_t offset = bootBlock->bytes_per_block;
    status_t status = OK; 

    if (numFAT < 2) {
        return OK;
    }

    // Read the first FAT as a reference
    uint8_t *fatReference = (uint8_t *)malloc(sizeOfFAT);
    if (fatReference == NULL) {
        return ERROR;
    }
    
    fseek(f, offset, SEEK_SET);
    fread(fatReference, 1, sizeOfFAT, f);

    // Compare with each additional FAT
    for (uint8_t i = 1; i < numFAT; i++) {
        uint8_t *currentFAT = (uint8_t *)malloc(sizeOfFAT);
        if (currentFAT == NULL) {
            status = ERROR;
            break;
        }

        fseek(f, offset + (i * sizeOfFAT), SEEK_SET);
        fread(currentFAT, 1, sizeOfFAT, f);

        // Compare the current FAT with the reference FAT
        if (memcmp(fatReference, currentFAT, sizeOfFAT) != 0) {
            status = ERROR;
            free(currentFAT);
            break;  
        }
        free(currentFAT);
    }
    
    free(fatReference);
    return status; 
}