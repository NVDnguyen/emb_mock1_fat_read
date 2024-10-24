#include <stdint.h>
#include <stdio.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
#include "fat_reader.h"
#include "fat_readdata.h"

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
   int n=0;
   for (int i = 0; i < lenentri; i++)
   {
      printf("Entry %d: %.*s.%.*s\n", i + 1, 8, arrentri[i].name, 3, arrentri[i].ext);
      printf("Attribute: %02x\n", arrentri[i].attr);
      printf("First Cluster: %d\n", arrentri[i].firstCluster);
      printf("File Size: %d bytes\n\n", arrentri[i].fileSize);
   }
   int aa=read_fat12_entry(arrentri[0].firstCluster,boot.bytes_per_block,0);
   uint32_t addata=(boot.blocks_per_fat*boot.num_fat+boot.num_root_dir_entries*32/512-1)*512;
   status_t aaaa=print_data(addata+arrentri[0].firstCluster*512,512);
}