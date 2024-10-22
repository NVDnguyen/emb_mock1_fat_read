#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat_file_explorer.h"

uint16_t next_cluster(uint16_t startingCluster, uint16_t seekOffset, FILE *f){
    
    // uint16_t hhh = 0x4200 + 0x200 * (entry->startingCluster - 0x0002);
    fseek(f,seekOffset,SEEK_SET);
    return 0;
}
void display_data(const BootBlock *boot, const DirectoryEntry *file){
    uint16_t fatOffSet = (boot->bytes_per_block - 0x0002)+ file->startingCluster*(uint16_t)(boot->filesystem_identifier); 
    
}

