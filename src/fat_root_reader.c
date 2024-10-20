#include "fat_root_reader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

status_t readDirectoryEntry(DirectoryEntry **dirEntries, BootBlock bootBlock)
{
    status_t status = OK;
    if (*dirEntries == NULL)
    {
        *dirEntries = (DirectoryEntry *)malloc(sizeof(DirectoryEntry) * bootBlock.num_root_dir_entries);
        if (*dirEntries == NULL)
        {
            return MEMORY_ALLOCATION_ERROR;
        }
    }

    FILE *f = fopen(PATH_FILE, "rb");
    if (f != NULL)
    {
        /* Point to the beginning of the root block */
        uint16_t rootBlockStartOffset = bootBlock.num_fat * bootBlock.blocks_per_fat * 512 + 512;
        printf("rootBlockStartOffset = %u\n",rootBlockStartOffset);
        printf("num_root_dir_entries = %u\n",bootBlock.num_root_dir_entries);
        printf("size of struct = %u\n",sizeof(DirectoryEntry));
        fseek(f, rootBlockStartOffset, SEEK_SET);

        /*Read each directory entry from the root block into dirEntries*/
        uint8_t j=0;  /*real dir*/  
        for (uint8_t i = 0; i < bootBlock.num_root_dir_entries; i++)
        {
            DirectoryEntry dir ;
            fread(&dir, sizeof(DirectoryEntry), 1, f);
            if(dir.attributes != 0x0F){
                (*dirEntries)[j] = dir;
                j++;
            }/*else it is long file name*/
        }

        fclose(f);
    }
    else
    {
        status = FILE_NULL_ERROR;
    }

    return status;
}

void printDirectoryEntries(const DirectoryEntry *dirEntries, uint16_t size) {
    if (dirEntries == NULL) {
        printf("No directory entries to print.\n");
        return;
    }

    for (uint16_t i = 0; i <4; i++) {
        const DirectoryEntry *entry = &dirEntries[i]; 

        printf("Directory Entry %u:\n", i + 1); 
        printf("Filename: %.8s\n", entry->filename);
        printf("Extension: %.3s\n", entry->extension);
        printf("Attributes: 0x%02X\n", entry->attributes);
        printf("Reserved: ");
        for (size_t j = 0; j < RESERVED_LENGTH; j++) {
            printf("%02X ", entry->reserved[j]);
        }
        printf("\n");
        printf("Time: 0x%04X\n", entry->time);
        printf("Date: 0x%04X\n", entry->date);
        printf("Starting Cluster: 0x%04X\n", entry->startingCluster);
        printf("File Size: 0x%08X bytes\n", entry->fileSize);
        printf("\n");  
    }
}

