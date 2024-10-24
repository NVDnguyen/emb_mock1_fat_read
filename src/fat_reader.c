#include "fat_reader.h"
#include <stdio.h>

uint16_t read_fat12_entry(uint16_t start_number_cluster, uint32_t fat_start, uint32_t n) {
    FILE *file = fopen(FILE_PATH, "rb");
    if (file == NULL) {
        return 0;  // Return 0 if the file cannot be opened
    }

    uint32_t fat_offset = (start_number_cluster * 3) / 2;

    // Move the file pointer to the calculated FAT offset
    if (fseek(file, fat_start + fat_offset + n, SEEK_SET) != 0) {
        fclose(file);  // Close the file in case of failure
        return 0;
    }

    // Read 2 bytes from the FAT table
    uint8_t entry_bytes[2];
    if (fread(entry_bytes, 1, 2, file) != 2) {
        fclose(file);  // Close the file in case of failure
        return 0;  // Return 0 if reading fails
    }

    // Decode the 12-bit FAT12 entry
    uint16_t entry;
    if (start_number_cluster % 2 == 0) {
        // Even cluster number
        entry = (entry_bytes[1] << 8) | entry_bytes[0];
        entry = entry & 0x0FFF;  // Extract the lower 12 bits
    } else {
        // Odd cluster number
        entry = (entry_bytes[1] << 4) | (entry_bytes[0] >> 4);
        entry = entry & 0x0FFF;  // Extract the lower 12 bits
    }

    fclose(file);  // Close the file after reading
    return entry;
}
