#include "fat_root_reader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
ParseTime parseTimeFromUint16(uint16_t rawTime) {
    ParseTime time;
    time.hours = (rawTime >> 11) & 0x1F;      // 5 bit từ bit 11-15
    time.minutes = (rawTime >> 5) & 0x3F;     // 6 bit từ bit 5-10
    time.seconds = rawTime & 0x1F;            // 5 bit từ bit 0-4 (mỗi giá trị là 2 giây)
    return time;
}
ParseDate parseDateFromUint16(uint16_t rawDate) {
    ParseDate date;
    date.year = (rawDate >> 9) & 0x7F;        // 7 bit từ bit 9-15
    date.month = (rawDate >> 5) & 0x0F;       // 4 bit từ bit 5-8
    date.day = rawDate & 0x1F;                // 5 bit từ bit 0-4
    return date;
}
status_t readDirectoryEntry(DirectoryEntry **dirEntries, BootBlock bootBlock)
{
    status_t status = OK;
    if (*dirEntries == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    FILE *f = fopen(FILE_PATH, "rb");
    if (f != NULL)
    {
        /* Point to the beginning of the root block */
        uint16_t rootBlockStartOffset = bootBlock.num_fat * bootBlock.blocks_per_fat * bootBlock.bytes_per_block + bootBlock.bytes_per_block;
 
        fseek(f, rootBlockStartOffset, SEEK_SET);
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
        status = ERROR_NULL_FILE;
    }

    return status;
}

void printDirectoryEntries(const DirectoryEntry *dirEntries, uint16_t size) {
    if (dirEntries == NULL) {
        printf("No directory entries to print.\n");
        return;
    }

    for (uint16_t i = 0; i <1; i++) {
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

        uint16_t hhh = 0x4200 + 0x200 * (entry->startingCluster - 0x0002);

        printf("Starting Cluster: 0x%04X - 0x%04X\n", entry->startingCluster,hhh);
        printf("File Size: 0x%08X bytes\n", entry->fileSize);
        printf("\n");  
    }
}


void display_all_file_info(const BootBlock *boot, DirectoryEntry **files, int fileCount) {
    printf("%-12s  %-12s  %-15s  %-12s  %-8s\n", "Filename", "Date Modified", "Type", "Attribute", "Size");

    for (int i = 0; i < fileCount; i++) {
        DirectoryEntry *file = files[i];
        ParseTime time = parseTimeFromUint16(file->time);
        ParseDate date = parseDateFromUint16(file->date);
        
        printf("%-12s  %-02u/%02u/%04u  %-15s  ", 
               file->filename, 
               date.day, 
               date.month, 
               date.year + 1980,
               "UNKNOWN");


        int found = 0;
        for (int j = 0; j < sizeof(attribute_strings) / sizeof(attribute_strings[0]); j++) {
            if (file->attributes & (1 << j)) {
                printf("%s ", attribute_strings[j]);
                found = 1;
            }
        }
        if (!found) {
            printf("NONE");
        }  
        printf("  %u bytes\n", file->fileSize);
    }
}


