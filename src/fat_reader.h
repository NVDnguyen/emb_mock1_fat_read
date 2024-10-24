#ifndef FAT_READER_H
#define FAT_READER_H
#include "status_code.h"
#include <stdint.h>
uint16_t read_fat12_entry(uint16_t start_number_cluster, uint32_t fat_start, uint32_t n);

#endif