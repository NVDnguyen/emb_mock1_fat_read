#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat_file_explorer.h"

uint16_t next_cluster(uint16_t startingCluster, uint16_t seekOffset, FILE *f)
{

    // uint16_t hhh = 0x4200 + 0x200 * (entry->startingCluster - 0x0002);
    fseek(f, seekOffset, SEEK_SET);
    return 0;
}
void display_data(const BootBlock *boot, const DirectoryEntry *file)
{
    status_t status;
    
    uint32_t fatOffSet; // Offset in the FAT table, use 32-bit to support all FAT types

    // Check the type of the filesystem (FAT12, FAT16, FAT32)
    if (strcpy(boot->filesystem_identifier, "FAT12")==0)
    {
        // Each entry in FAT12 is 1.5 bytes (12 bits)
        fatOffSet = file->startingCluster * 3 / 2;
    }
    else if (strcpy(boot->filesystem_identifier, "FAT16")==0)
    {
        // Each entry in FAT16 is 2 bytes (16 bits)
        fatOffSet = file->startingCluster * 2;
    }
    else if (strcpy(boot->filesystem_identifier, "FAT32")==0)
    {
        // Each entry in FAT32 is 4 bytes (32 bits)
        fatOffSet = file->startingCluster * 4;
    }
    else
    {
        // Error handling for unsupported or unknown filesystem type
        printf("Unsupported filesystem type.\n");
        status = ERROR_UNSUPPORT_FILE_SYSTEM;        
    }
}
