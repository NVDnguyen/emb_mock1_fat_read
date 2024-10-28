#include "fat_root_reader.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "console_utils.h"
ParseTime parseTimeFromUint16(uint16_t rawTime)
{
    ParseTime time;
    time.hours = (rawTime >> 11) & 0x1F;
    time.minutes = (rawTime >> 5) & 0x3F;
    time.seconds = rawTime & 0x1F;
    return time;
}
ParseDate parseDateFromUint16(uint16_t rawDate)
{
    ParseDate date;
    date.year = (rawDate >> 9) & 0x7F;
    date.month = (rawDate >> 5) & 0x0F;
    date.day = rawDate & 0x1F;
    return date;
}

uint8_t readFolder(FILE *f, DirectoryEntry *dirEntries, uint16_t num_root_dir_entries, uint32_t rootDirectoryLocation)
{
    uint8_t j = 0; /*real dir*/
    if (f != NULL)
    {
        fseek(f, rootDirectoryLocation, SEEK_SET);
        for (uint8_t i = 0; i < num_root_dir_entries; i++)
        {
            DirectoryEntry dir;
            fread(&dir, sizeof(DirectoryEntry), 1, f);

            (dirEntries)[j] = dir;
            j++;
        }
    }
    return j;
}
