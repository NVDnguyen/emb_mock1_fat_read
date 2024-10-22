#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fat_file_explorer.h"

status_t next_cluster(uint16_t startingCluster, BootBlock boot, uint8_t *arr)
{
    status_t status;
    if (arr == NULL)
    {
        status = ERROR;
    }
    else
    {
        uint16_t nextCluster;
        FILE *f = fopen(FILE_PATH,"rb");
        if(f!=NULL){
            // fs

        }else{
            status = ERROR_NULL_FILE;
        }
    }
}
