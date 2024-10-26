#include"fat_root_reader.h"
#include <stdio.h>
#include <string.h>
status_t readRoot(DirectoryEntry *arr, uint16_t rootEntryCount, uint32_t rootDirByteOffset, int *len)
{
    *len=0;
    status_t status = OK;
    FILE *f = fopen(FILE_PATH, "rb");
    if (f == NULL) {
        return ERROR_NULL_FILE; 
    }

    fseek(f, rootDirByteOffset, SEEK_SET);
    DirectoryEntry entry;

    for (int i = 0; i < rootEntryCount; i++) {
        (*len)++; 
        fread(&entry, 32, 1, f);


        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5 || entry.attr == 0x0F) {
            (*len)--; 
            continue;
        }
        arr[*len-1] = entry;
    }
    
    fclose(f);
    return status;
}
status_t readFolder(DirectoryEntry *arr, uint16_t rootEntryCount, uint32_t rootDirByteOffset, int *len)
{
    *len=0;
    status_t status = OK;
    FILE *f = fopen(FILE_PATH, "rb");
    if (f == NULL) {
        return ERROR_NULL_FILE; 
    }

    fseek(f, rootDirByteOffset, SEEK_SET);
    DirectoryEntry entry;

    for (int i = 0; i < rootEntryCount; i++) {
        (*len)++; 
        fread(&entry, 32, 1, f);


        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5 || entry.attr == 0x0F) {
            (*len)--; 
            continue;
        }
        arr[*len-1] = entry;
    }
    
    fclose(f);
    return status;
}
void print_directory_entry(const DirectoryEntry *entry) {
    // Print file name
    char temp_name[9]; 
    strncpy(temp_name, entry->name, 8); 
    temp_name[8] = '\0'; 
    printf("%-18s", temp_name);

    //printf("%-18s", entry->name); 
    // Print file extension
    if (entry->ext[0] != 32) { 
        printf("%-10s  ", entry->ext);
    } else {
        printf("            ");
    }

    // Print attributes
    printf("%-8x   ", entry->attr);

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

void print_allentri(int lenentri, DirectoryEntry *arrentri) {
    printf("Index   File Name      Extension   Attributes      Time         Date           File Size\n");
    printf("----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lenentri; i++) {
        printf("%-8d", i + 1); // In chỉ số
        print_directory_entry(&arrentri[i]);
    }
}