#include "fat_root_reader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_utils.h" 
ParseTime parseTimeFromUint16(uint16_t rawTime)
{
    ParseTime time;
    time.hours = (rawTime >> 11) & 0x1F; 
    time.minutes = (rawTime >> 5) & 0x3F; 
    time.seconds = rawTime & 0x1F;      
    return time;
}
ParseDate parseDateFromUint16(uint16_t rawDate)
{
    ParseDate date;
    date.year = (rawDate >> 9) & 0x7F; 
    date.month = (rawDate >> 5) & 0x0F; 
    date.day = rawDate & 0x1F;         
    return date;
}
//readRootEntry(dirEntries, boot.num_root_dir_entries, adroot, &numEntry);
uint8_t readRootEntry(FILE *f,DirectoryEntry *dirEntries,uint16_t num_root_dir_entries, uint32_t adroot)
{
    uint8_t j = 0; /*real dir*/
    if (f != NULL)
    {
        fseek(f, adroot, SEEK_SET);
      
        for (uint8_t i = 0; i < num_root_dir_entries; i++)
        {
            DirectoryEntry dir;
            fread(&dir, sizeof(DirectoryEntry), 1, f);
            if (dir.attributes != 0x0F && (strcmp(dir.filename, "") != 0 || dir.fileSize != 0))
            {
                (dirEntries)[j] = dir;
                j++;
            } /*else it is long file name*/
        }
        

       
    }
    return j;
   
}



uint8_t readFolder(FILE *f,DirectoryEntry *arr, uint16_t rootEntryCount, uint32_t rootDirByteOffset)
{
    uint8_t len=0;
    status_t status = OK;

    fseek(f, rootDirByteOffset, SEEK_SET);
    DirectoryEntry entry;

    for (int i = 0; i < rootEntryCount; i++) {
        (len)++; 
        fread(&entry, 32, 1, f);


        if (entry.filename[0] == 0x00 || entry.filename[0] == 0xE5 || entry.attributes == 0x0F) {
            (len)--; 
            continue;
        }
        arr[len-1] = entry;
    }
    
    return len;
}


void print_directory_entry(const DirectoryEntry *entry) {
    // Print file name
    char temp_name[9]; 
    strncpy(temp_name, entry->filename, 8); 
    temp_name[8] = '\0'; 
    printf("%-18s", temp_name);

    //printf("%-18s", entry->name); 
    // Print file extension
    if (entry->extension[0] != 32) { 
        printf("%-10s  ", entry->extension);
    } else {
        printf("            ");
    }

    // Print attributes
    //printf("%-9s   ", attribute_strings[entry->attributes]);
    printAttributes(entry->attributes);
    printf("        ");
    // Decode and print time
    uint8_t seconds = (entry->time & 0x1F) * 2; 
    uint8_t minutes = (entry->time >> 5) & 0x3F; 
    uint8_t hours = (entry->time >> 11) & 0x1F; 
    printf("%02u:%02u:%02u     ", hours, minutes, seconds);

    // Decode and print date
    uint8_t day = entry->date & 0x1F; 
    uint8_t month = (entry->date >> 5) & 0x0F; 
    uint16_t year = ((entry->date >> 9) & 0x7F) + 1980; 
    printf("%02u/%02u/%04u", day, month, year);


    // Print file size
    printf("%10u bytes\n", entry->fileSize); 
}

void print_allentri( DirectoryEntry *arrentri,uint16_t size) {
    printf("Index   File Name      Extension   Attributes      Time         Date           File Size\n");
    printf("----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < size; i++) {
        printf("%-8d", i + 1); // In chỉ số
        print_directory_entry(&arrentri[i]);
    }
}
void printAttributes(uint8_t attributes) {
    switch (attributes) {
        case 0x01:
            printf("r   ");
            break;
        case 0x02:
            printf("hidden");
            break;
        case 0x04:
            printf("s   ");
            break;
        case 0x08:
            printf("vl  ");
            break;
        case 0x10:
            printf("dir ");
            break;
        case 0x20:
            printf("a   ");
            break;
        case 0x40:
            printf("U1  ");
            break;
        case 0x80:
            printf("U2");
            break;
        default:
            printf(" -  ");
            break;
    }
}
