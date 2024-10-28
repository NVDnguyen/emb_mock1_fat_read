#ifndef __PARSEDATA_H__
#define __PARSEDATA_H__ 

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
/* Define ASCII identifiers for FAT types */
#define FAT12 "FAT12"
#define FAT16 "FAT16"
#define FAT32 "FAT32"

uint8_t isFilesystemType(const char *identifier, const char *type);
uint32_t getNextCluster(uint32_t cluster,uint32_t fatStartOffset, FILE *f, const char *filesystem_identifier);
uint32_t NextIndexCluster_FAT(uint32_t cluster,uint32_t fatStartOffset , const char *filesystem_identifier);
/*display*/
void displayDataInFile(uint32_t startCluster, FILE *f, BootBlock bootBlock, uint16_t fileSize);
void print_allentri( DirectoryEntry *arrentri,uint16_t size);
void printAttributes(uint8_t attributes);

/*remove*/
status_t removeFile(DirectoryEntry dri ,uint32_t offSet, uint16_t indexCluster,FILE *f);
status_t makeDir(uint32_t offSet, uint16_t indexCluster,FILE *f);


#endif   /*__PARSEDATA_H__ */