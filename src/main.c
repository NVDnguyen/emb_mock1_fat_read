#include <stdint.h>
#include <stdio.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"


int main()
{
   status_t status;
   BootBlock boot;
   status = readBootBlock(&boot) ;
   printf("\n%s\n",status_strings[status]);
   DirectoryEntry arrentri[boot.num_root_dir_entries];
   uint32_t adroot= (boot.num_fat*boot.blocks_per_fat+1)*boot.bytes_per_block;
   printf("%d\n",adroot);
   int lenentri=0;
   printf("%d\n",boot.num_root_dir_entries);
   readRoot(arrentri,boot.num_root_dir_entries,adroot,&lenentri);
   printf("%d\n",lenentri);
   for (int i = 0; i < lenentri; i++)
   {
      printf("Entry %d: %.*s.%.*s\n", i + 1, 8, arrentri[i].name, 3, arrentri[i].ext);
        printf("Attribute: %02x\n", arrentri[i].attr);
        printf("First Cluster: %d\n", arrentri[i].firstCluster);
        printf("File Size: %d bytes\n\n", arrentri[i].fileSize);
   }
   
}