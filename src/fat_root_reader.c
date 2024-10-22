#include"fat_root_reader.h"
#include <stdio.h>
status_t readRoot(DirectoryEntry *arr, uint16_t rootEntryCount, uint32_t rootDirByteOffset, int *len)
{
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
