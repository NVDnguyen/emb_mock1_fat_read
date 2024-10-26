#include "fat_data_reader.h"
#include <string.h>
#include <stdio.h>
/* Define ASCII identifiers for FAT types */
#define FAT12 "FAT12"
#define FAT16 "FAT16"
#define FAT32 "FAT32"

/* Function to compare the last 5 characters of the filesystem identifier */
uint8_t isFilesystemType(const char *identifier, const char *type)
{
    return strncmp(identifier, type, 5) == 0;
}
status_t isClusterValid(uint32_t cluster, uint32_t endOfClusterMarker)
{
    if (cluster < 2 || cluster >= endOfClusterMarker)
    {
        return ERROR;
    }
    return OK;
}

uint32_t getNextCluster(uint32_t cluster, uint32_t fatStartOffset, FILE *f, const char *filesystem_identifier)
{
    uint32_t result = 0;

    if (isFilesystemType(filesystem_identifier, FAT12))
    {
        uint8_t str[2]; /* Store 2 bytes from the FAT */
        uint8_t str_hex[4];
        fseek(f, NextIndexCluster_FAT(cluster, fatStartOffset, FAT12), SEEK_SET);

        /* Read 2 bytes from the FAT */
        for (uint8_t i = 0; i < 2; i++)
        {
            str[i] = getc(f);
        }

        /* Convert to 12-bit value */
        uint8_t a = cluster % 2;
        str_hex[0] = ((str[0] >> 4) & 0x0F);
        str_hex[1] = (str[0] & 0x0F);
        str_hex[2] = ((str[1] >> 4) & 0x0F);
        str_hex[3] = (str[1] & 0x0F);

        if (a == 1)
        {
            result = (str_hex[2] << 8) + (str_hex[3] << 4) + str_hex[0];
        }
        else
        {
            result = (str_hex[3] << 8) + (str_hex[0] << 4) + str_hex[1];
        }
    }
    else if (isFilesystemType(filesystem_identifier, FAT16))
    {
        fseek(f, NextIndexCluster_FAT(cluster, fatStartOffset, FAT16), SEEK_SET);
        uint16_t fat_entry;
        fread(&fat_entry, sizeof(uint16_t), 1, f);
        result = fat_entry;
    }
    else if (isFilesystemType(filesystem_identifier, FAT32))
    {
        fseek(f, NextIndexCluster_FAT(cluster, fatStartOffset, FAT32), SEEK_SET);
        uint32_t fat_entry;
        fread(&fat_entry, sizeof(uint32_t), 1, f);
        result = fat_entry & 0x0FFFFFFF;
    }

    return result;
}

uint32_t NextIndexCluster_FAT(uint32_t cluster, uint32_t fatStartOffset, const char *filesystem_identifier)
{
    uint32_t result = 0;
    if (isFilesystemType(filesystem_identifier, FAT12))
    {
        result = (cluster * 3 / 2) + fatStartOffset;
    }
    else if (isFilesystemType(filesystem_identifier, FAT16))
    {
        result = cluster * 2 + fatStartOffset;
    }
    else if (isFilesystemType(filesystem_identifier, FAT32))
    {
        result = cluster * 4 + fatStartOffset;
    }
    return result;
}

void displayDataInFile(uint32_t startCluster, FILE *f, BootBlock bootBlock, uint16_t fileSize)
{

    uint32_t endOfClusterMarker;

    if (isFilesystemType(bootBlock.filesystem_identifier, FAT12))
    {
        endOfClusterMarker = 0xFF8;
    }
    else if (isFilesystemType(bootBlock.filesystem_identifier, FAT16))
    {
        endOfClusterMarker = 0xFFF8;
    }
    else if (isFilesystemType(bootBlock.filesystem_identifier, FAT32))
    {
        endOfClusterMarker = 0xFFFFFF8;
    }
    else
    {
        return;
    }
    /**/
    uint16_t dataBlockStartOffset =
        (bootBlock.num_fat * bootBlock.blocks_per_fat + 1) * bootBlock.bytes_per_block +
        bootBlock.num_root_dir_entries * 32;
    uint32_t clusterSize = bootBlock.bytes_per_block;

    uint16_t currentFAT = 1;
    uint32_t fatStartOffset = bootBlock.bytes_per_block + currentFAT; /*first FAT table*/

    uint32_t currentCluster = startCluster;
    uint32_t nextCluster;

    uint32_t totalBytesRead = 0;
    State_t state = PROCESSING;

    while (state != EXIT)
    {
        if (state == PROCESSING)
        {
            uint32_t dataOffset = dataBlockStartOffset + (currentCluster - 2) * clusterSize;
            fseek(f, dataOffset, SEEK_SET);

            uint8_t buffer[clusterSize];
            fread(buffer, sizeof(uint8_t), clusterSize, f);

            /* Display data from the cluster */
            for (uint32_t i = 0; i < clusterSize; i++)
            {
                if (totalBytesRead >= fileSize)
                {
                    state = EXIT;
                    break;
                }
                printf("%c", buffer[i]);
                totalBytesRead++;
            }
            printf("\n");
        }

        if (state != EXIT)
        {

            nextCluster = getNextCluster(currentCluster, fatStartOffset, f, bootBlock.filesystem_identifier);
            
            if (nextCluster >= endOfClusterMarker || nextCluster == currentCluster || nextCluster == 0x00)
            {
                //printf("\n currentFAT = %d > bootBlock.num_fat = %d : next= %02X\n ", currentFAT, bootBlock.num_fat, nextCluster);

                currentFAT++;
                if (currentFAT > bootBlock.num_fat)
                {
                    state = EXIT;
                    break;
                }
                else
                {
                    fatStartOffset = clusterSize * currentFAT;
                    state = REGET;
                }
            }
            else
            {
                currentCluster = nextCluster;
                state = PROCESSING;
            }

            /*if dont check FAT uncomment this*/
            // currentCluster = nextCluster;
            // state = PROCESSING;
        }
    }
}
