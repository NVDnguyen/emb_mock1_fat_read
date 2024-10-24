#ifndef __PARSEDATA_H__
#define __PARSEDATA_H__ 

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t getNextCluster (uint32_t cluster);
uint32_t NextIndexCluster_FAT (uint32_t cluster);
uint32_t NextIndexDataofCluster (uint32_t cluster);


#endif   // __PARSEDATA_H__ 