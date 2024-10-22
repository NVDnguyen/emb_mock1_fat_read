#include <stdio.h>
#include <stdint.h>
#include "Cluster_fat.h"

FILE* fileptr;  // Global file pointer to manage the FAT file

int main()
{
    // Declare variables
    uint32_t start_cluster = 5;  // Example starting cluster, you can change this value
    uint32_t fat_value;
    uint32_t data_address;
    uint32_t fat_address;

    // Open the FAT file (disk image)
    fileptr = fopen("floppy.img", "rb");  // Ensure that you have a valid "fat_image.bin" file for testing

    if (fileptr == NULL) 
    {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    // Calculate the data address of the starting cluster
    data_address = ClusterToAdd_Data(start_cluster);
    printf("Data address of starting cluster (cluster %u): 0x%X\n", start_cluster, data_address);

    // Calculate the FAT address of the starting cluster
    fat_address = ClusterToAdd_Fat(start_cluster);
    printf("FAT address of starting cluster (cluster %u): 0x%X\n", start_cluster, fat_address);

    // Retrieve the next FAT value from the starting cluster
    fat_value = Handle_Fat(start_cluster);
    printf("Next FAT value for starting cluster (cluster %u): 0x%X\n", start_cluster, fat_value);

    // Close the file
    fclose(fileptr);

    return 0;
}
