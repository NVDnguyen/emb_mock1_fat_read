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


uint8_t readFolder(FILE *f, DirectoryEntry *arr, uint16_t rootEntryCount, uint32_t rootDirByteOffset)
{
    uint8_t len = 0;
    status_t status = OK;

    fseek(f, rootDirByteOffset, SEEK_SET);
    DirectoryEntry entry;

    for (int i = 0; i < rootEntryCount; i++)
    {
        (len)++;
        fread(&entry, 32, 1, f);

        if (entry.filename[0] == 0x00 || entry.filename[0] == 0xE5 || entry.attributes == 0x0F)
        {
            (len)--;
            continue;
        }
        arr[len - 1] = entry;
    }

    return len;
}

