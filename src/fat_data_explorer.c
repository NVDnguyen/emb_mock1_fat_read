#include "fat_data_explorer.h"
#include <string.h>
#include <stdio.h>
uint32_t getEndOfClusterMarker(const char *filesystem_identifier)
{
    if (isFilesystemType(filesystem_identifier, FAT12))
    {
        return 0xFF8;
    }
    else if (isFilesystemType(filesystem_identifier, FAT16))
    {
        return 0xFFF8;
    }
    else if (isFilesystemType(filesystem_identifier, FAT32))
    {
        return 0xFFFFFF8;
    }
    else
    {
        return 0; // Indicate invalid filesystem
    }
}
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

    uint32_t endOfClusterMarker = getEndOfClusterMarker(bootBlock.filesystem_identifier);
    if (endOfClusterMarker == 0)
    {
        return;
    }

    /**/
    uint16_t dataBlockStartOffset =
        (bootBlock.num_fat * bootBlock.blocks_per_fat + 1) * bootBlock.bytes_per_block +
        bootBlock.num_root_dir_entries * 32;
    uint32_t clusterSize = bootBlock.bytes_per_block;

    uint16_t currentFAT = 1;
    uint32_t fatStartOffset = bootBlock.bytes_per_block; /*first FAT table*/

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

            if (nextCluster >= endOfClusterMarker || nextCluster == currentCluster || nextCluster == 0)
            {
                printf("\n currentFAT = %d > bootBlock.num_fat = %d : next= %02X\n ", currentFAT, bootBlock.num_fat, nextCluster);

                currentFAT++;
                if (currentFAT > bootBlock.num_fat)
                {
                    state = EXIT;
                    break;
                }
                else
                {
                    fatStartOffset += bootBlock.bytes_per_block * bootBlock.blocks_per_fat; /*1,2,...*/
                    state = REGET;
                }
            }
            else
            {
                currentCluster = nextCluster;
                state = PROCESSING;
            }
        }
    }
}
void print_directory_entry(const DirectoryEntry *entry)
{
    // Print file name
    char temp_name[9];
    strncpy(temp_name, entry->filename, 8);
    temp_name[8] = '\0';
    printf("%-18s", temp_name);

    // printf("%-18s", entry->name);
    //  Print file extension
    if (entry->extension[0] != 32)
    {
        printf("%-10s  ", entry->extension);
    }
    else
    {
        printf("            ");
    }

    // Print attributes
    // printf("%-9s   ", attribute_strings[entry->attributes]);
    printAttributes(entry->attributes);
    printf("        ");
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

void print_allentri(DirectoryEntry *arrentri, uint16_t size)
{
    printf("File Name      Extension   Attributes      Time         Date           File Size\n");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++)
    {
        uint8_t firstB = (uint8_t)arrentri[i].filename[0];

        if (firstB != 0 && firstB != 0xE5 && arrentri[i].attributes != 0x0f )
        {
            print_directory_entry(&arrentri[i]);
        }
    }
}
void printAttributes(uint8_t attributes)
{
    switch (attributes)
    {
    case 0x01:
        printf("r   ");
        break;
    case 0x02:
        printf("hidden");
        break;
    case 0x04:
        printf("s   ");
        break;
    case 0x08:
        printf("vl  ");
        break;
    case 0x10:
        printf("dir ");
        break;
    case 0x20:
        printf("a   ");
        break;
    case 0x40:
        printf("U1  ");
        break;
    case 0x80:
        printf("U2");
        break;
    default:
        printf(" -  ");
        break;
    }
}
status_t removeFile(DirectoryEntry *dir, uint32_t dirOffset, uint16_t indexCluster, FILE *f, BootBlock *boot)
{
    if (f == NULL)
    {
        return ERROR_NULL_FILE;
    }
    if (fseek(f, dirOffset + indexCluster * sizeof(DirectoryEntry), SEEK_SET) != 0)
    {
        return ERROR_READ;
    }

    /*Mark the file as deleted in the directory entry*/
    dir->filename[0] = 0xE5;

    if (fwrite(dir, sizeof(DirectoryEntry), 1, f) != 1)
    {
        return ERROR_WRITE;
    }

    uint32_t firstCluster = dir->startingCluster;
    if (firstCluster != 0)
    {
        uint32_t fatStartOffset = boot->bytes_per_block;

        /*Iterate over each FAT table to mark the cluster as free*/
        for (int i = 0; i < boot->num_fat; i++)
        {
            uint32_t fatOffsetForCluster = fatStartOffset + (i * boot->blocks_per_fat * boot->bytes_per_block);

            /*Seek to the location of the first cluster in the FAT*/
            if (fseek(f, fatOffsetForCluster + NextIndexCluster_FAT(firstCluster, fatStartOffset, boot->filesystem_identifier), SEEK_SET) != 0)
            {
                return ERROR_READ;
            }

            /*Write a zero value to mark the cluster as free*/
            uint32_t freeCluster = 0x00000000;
            if (fwrite(&freeCluster, sizeof(uint32_t), 1, f) != 1)
            {
                return ERROR_WRITE;
            }
        }
    }

    return OK;
}

status_t makeDir(uint32_t offSet, uint16_t indexCluster, FILE *f)
{
    if (f == NULL)
    {
        return ERROR_NULL_FILE;
    }

    if (fseek(f, offSet + indexCluster * sizeof(DirectoryEntry), SEEK_SET) != 0)
    {
        return ERROR_READ;
    }

    DirectoryEntry dir;

    if (fwrite(&dir, sizeof(DirectoryEntry), 1, f) != 1)
    {
        return ERROR;
    }

    return OK;
}