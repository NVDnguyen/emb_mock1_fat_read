#ifndef __CLUSTER_FAT_H__
#define __CLUSTER_FAT_H__

#include <stdint.h>
#include <stdio.h>

uint32_t Handle_Fat(uint32_t cluster);
uint32_t ClusterToAdd_Fat(uint32_t cluster);
uint32_t ClusterToAdd_Data(uint32_t cluster);

#endif
