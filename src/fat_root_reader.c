#include "fat_root_reader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_utils.h" 
ParseTime parseTimeFromUint16(uint16_t rawTime)
{
    ParseTime time;
    time.hours = (rawTime >> 11) & 0x1F;  // 5 bit từ bit 11-15
    time.minutes = (rawTime >> 5) & 0x3F; // 6 bit từ bit 5-10
    time.seconds = rawTime & 0x1F;        // 5 bit từ bit 0-4 (mỗi giá trị là 2 giây)
    return time;
}
ParseDate parseDateFromUint16(uint16_t rawDate)
{
    ParseDate date;
    date.year = (rawDate >> 9) & 0x7F;  // 7 bit từ bit 9-15
    date.month = (rawDate >> 5) & 0x0F; // 4 bit từ bit 5-8
    date.day = rawDate & 0x1F;          // 5 bit từ bit 0-4
    return date;
}

status_t readRootEntry(DirectoryEntry *dirEntries,FILE *f , BootBlock bootBlock)
{
    uint8_t size=0; 
 
    if (f != NULL)
    {

        /* Point to the beginning of the root block */
        uint16_t rootBlockStartOffset = bootBlock.num_fat * bootBlock.blocks_per_fat * bootBlock.bytes_per_block + bootBlock.bytes_per_block;

        fseek(f, rootBlockStartOffset, SEEK_SET);
        uint8_t j = 0; /*real dir*/
        for (uint8_t i = 0; i < bootBlock.num_root_dir_entries; i++)
        {
            DirectoryEntry dir;
            fread(&dir, sizeof(DirectoryEntry), 1, f);
            if (dir.attributes != 0x0F && (strcmp(dir.filename, "") != 0 || dir.fileSize != 0))
            {
                (dirEntries)[j] = dir;
                j++;
            } /*else it is long file name*/
        }
        size = j;

       
    }
    return size;
   
}

void printSingleDE(DirectoryEntry d)
{
    // Parse time and date from entry
    ParseTime time = parseTimeFromUint16(d.time);
    ParseDate date = parseDateFromUint16(d.date);

    // Print serial number, filename, extension, date, and time
    printf("%-20s  %-8s  %-02u/%02u/%04u       %-02u:%02u:%02u  ",
           d.filename,
           d.extension,
           date.day,
           date.month,
           date.year + 1980,
           time.hours,
           time.minutes,
           time.seconds * 2);

    // Print file attributes
    int found = 0;
    for (int j = 0; j < sizeof(attribute_strings) / sizeof(attribute_strings[0]); j++)
    {
        if (d.attributes & (1 << j))
        {
            printf("    %s ", attribute_strings[j]);
            found = 1;
        }
    }
    if (!found)
    {
        printf("    NONE");
    }

    // Print file size
    printf("        %-10u\n", d.fileSize);
}

void printDirectoryEntries(const DirectoryEntry *dirEntries, uint16_t size)
{
    if (dirEntries == NULL)
    {
        printf("No directory entries to print.\n");
        return;
    }

    // Print header with better alignment
    printf("%-5s  %-20s  %-8s  %-15s  %-8s  %-12s  %-10s\n",
           "No.", "Filename", "Extension", "Date Modified", "Time", "Attribute", "Size");

    // Print a separator line
    printf("%-5s  %-20s  %-8s  %-15s  %-8s  %-12s  %-10s\n",
           "----", "--------------------", "--------", "---------------", "--------", "------------", "----------");

    for (uint16_t i = 0; i < size; i++)
    {
        const DirectoryEntry *entry = &dirEntries[i];

        // Parse time and date from entry
        ParseTime time = parseTimeFromUint16(entry->time);
        ParseDate date = parseDateFromUint16(entry->date);

        // Print serial number, filename, extension, date, and time
        printf("%-5u  %-20s  %-8s  %-02u/%02u/%04u       %-02u:%02u:%02u  ",
               i + 1, // Serial number starts from 1
               entry->filename,
               entry->extension,
               date.day,
               date.month,
               date.year + 1980,
               time.hours,
               time.minutes,
               time.seconds * 2 // Convert seconds from 0-29 to real seconds (0-58)
        );

        // Print file attributes
        int found = 0;
        for (int j = 0; j < sizeof(attribute_strings) / sizeof(attribute_strings[0]); j++)
        {
            if (entry->attributes & (1 << j))
            {
                printf("    %s ", attribute_strings[j]);
                found = 1;
            }
        }
        if (!found)
        {
            printf("    NONE");
        }

        // Print file size
        printf("        %-10u\n", entry->fileSize);
    }
}

void printDirectoryEntries2(const DirectoryEntry *dirEntries, uint16_t size)
{
    if (dirEntries == NULL)
    {
        setColor(COLOR_LIGHT_RED, COLOR_BLACK);
        printf("No directory entries to print.\n");
        setColor(COLOR_DEFAULT, COLOR_BLACK);
        return;
    }

    // Print header with better alignment
    setColor(COLOR_LIGHT_CYAN, COLOR_BLACK);
    printf("%-5s  %-20s  %-8s  %-15s  %-8s  %-12s  %-10s\n",
           "No.", "Filename", "Extension", "Date Modified", "Time", "Attribute", "Size");

    // Print a separator line
    setColor(COLOR_LIGHT_GRAY, COLOR_BLACK);
    printf("%-5s  %-20s  %-8s  %-15s  %-8s  %-12s  %-10s\n",
           "----", "--------------------", "--------", "---------------", "--------", "------------", "----------");
    setColor(COLOR_DEFAULT, COLOR_BLACK);

    for (uint16_t i = 0; i < size; i++)
    {
        const DirectoryEntry *entry = &dirEntries[i];

        // Parse time and date from entry
        ParseTime time = parseTimeFromUint16(entry->time);
        ParseDate date = parseDateFromUint16(entry->date);

        // Print serial number, filename, extension, date, and time
        setColor(COLOR_LIGHT_GREEN, COLOR_BLACK);
        printf("%-5u", i + 1); // Serial number starts from 1

        setColor(COLOR_YELLOW, COLOR_BLACK);
        printf("  %-20s", entry->filename);
        printf("  %-8s", entry->extension);

        setColor(COLOR_LIGHT_CYAN, COLOR_BLACK);
        printf("  %02u/%02u/%04u", date.day, date.month, date.year + 1980);
        printf("       %02u:%02u:%02u", time.hours, time.minutes, time.seconds * 2);

        // Print file attributes
        setColor(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
        int found = 0;
        for (int j = 0; j < sizeof(attribute_strings) / sizeof(attribute_strings[0]); j++)
        {
            if (entry->attributes & (1 << j))
            {
                printf("    %s ", attribute_strings[j]);
                found = 1;
            }
        }
        if (!found)
        {
            printf("    NONE");
        }

        // Print file size
        setColor(COLOR_LIGHT_BLUE, COLOR_BLACK);
        printf("        %-10u B\n", entry->fileSize);
    }

    // Reset color to default
    setColor(COLOR_DEFAULT, COLOR_BLACK);
}

void parseEntry(unsigned char *data, DirectoryEntry *entry) {
    memcpy(entry->filename, data, FILENAME_LENGTH);

    memcpy(entry->extension, data + FILENAME_LENGTH, EXTENSION_LENGTH);

    entry->attributes = data[FILENAME_LENGTH + EXTENSION_LENGTH + 11];
    memcpy(entry->reserved, data + FILENAME_LENGTH + EXTENSION_LENGTH + 12, RESERVED_LENGTH);
    
    entry->time = *(uint16_t *)(data + FILENAME_LENGTH + EXTENSION_LENGTH + 12 + RESERVED_LENGTH);
    entry->date = *(uint16_t *)(data + FILENAME_LENGTH + EXTENSION_LENGTH + 12 + RESERVED_LENGTH + 2);
    entry->startingCluster = *(uint16_t *)(data + FILENAME_LENGTH + EXTENSION_LENGTH + 12 + RESERVED_LENGTH + 4);
    entry->fileSize = *(uint32_t *)(data + FILENAME_LENGTH + EXTENSION_LENGTH + 12 + RESERVED_LENGTH + 6);
}
