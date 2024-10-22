#ifndef FAt_ROOT_READER_H
#define FAt_ROOT_READER_H

#include <stdint.h>
#include "status_code.h"
#include "fat_boot_reader.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FILENAME_LENGTH 8
#define EXTENSION_LENGTH 3
#define RESERVED_LENGTH 10 

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
typedef struct DirectoryEntry{
    char filename[FILENAME_LENGTH];
    char extension[EXTENSION_LENGTH];
    uint8_t attributes;
    uint8_t reserved[RESERVED_LENGTH];
    uint16_t time;
    uint16_t date;
    uint16_t startingCluster;
    uint32_t fileSize;
} __attribute__((packed)) DirectoryEntry;


/*
 * @param dirEntries Array of DirectoryEntry elements to store array of information about files/directories read from the root directory.
 * @param bootBlock BootBlock structure containing information from the boot block of the file system, used to determine the location and size of the root directory.
 *
 * @return status_t
 *         - OK if the read operation is successful.
 *         - ERROR_NULL_FILE if the file cannot be opened.
 *         - ERROR_READ if an error occurs while reading the data.
 *
 */
status_t readDirectoryEntry(DirectoryEntry **dirEntries, BootBlock bootBlock);
void printDirectoryEntries(const DirectoryEntry *dirEntries, uint16_t size);

#endif /*FAt_ROOT_READER_H*/