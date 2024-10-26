#ifndef __PARSEDATA_H__
#define __PARSEDATA_H__ 

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fat_boot_reader.h"

uint32_t getNextCluster (uint32_t cluster,FILE *f, const char *filesystem_identifier);
uint32_t NextIndexCluster_FAT (uint32_t cluster, const char *filesystem_identifier);

void displayDataInFile(uint32_t startCluster,FILE *f,BootBlock boot);

#endif   /*__PARSEDATA_H__ */