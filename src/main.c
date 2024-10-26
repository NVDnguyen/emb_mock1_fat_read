#include <stdint.h>
#include <stdio.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
#include "fat_reader.h"
#include "fat_readdata.h"
void print_allfile(DirectoryEntry *arrentri,BootBlock boot,uint32_t addata,uint32_t chose){
   int n=0;
   uint32_t aa=arrentri[chose-1].firstCluster;
   while (aa< 0xFF8)
   {
      print_data(addata+aa*boot.bytes_per_block,boot.bytes_per_block);
      aa=read_fat12_entry(arrentri[chose-1].firstCluster,boot.bytes_per_block,n);
      n++;
   }
}
void print_control_instructions(state_t a){
   printf("\n\n");
   if (a==INFOLDER)
   {
      printf("0: Stop program\n\n");
   }
   if (a==INFILE)
   {
      printf("0: Exit file\n\n");
   }
   
   
}
int main()
{
   state_t state= INFOLDER;
   printf("\033[H\033[J");
   uint32_t chose;
   status_t status;
   BootBlock boot;//Boot Block
   //Read boot block
   status = readBootBlock(&boot) ;
   //Create array save entris in current file
   DirectoryEntry arrentri[boot.num_root_dir_entries];
   //Address's root dir
   uint32_t adroot= (boot.num_fat*boot.blocks_per_fat+1)*boot.bytes_per_block;
   int lenentri=0;
   //Read roor dir
   readRoot(arrentri,boot.num_root_dir_entries,adroot,&lenentri);
   int n=0;
   //print all file in root dir
   print_allentri(lenentri,arrentri);
   uint32_t addata=(boot.blocks_per_fat*boot.num_fat+boot.num_root_dir_entries*32/boot.bytes_per_block-1)*boot.bytes_per_block;//address area data
   print_control_instructions(state);

   while (state!=CLOSEE)
   {
      scanf("%d",&chose);
      switch (state)
      {
         
         
         case INFOLDER:
            if (0<chose&&chose<=lenentri)
            {
               // file
               if (arrentri[chose-1].attr==0x0)
               {
                  printf("\033[H\033[J");
                  print_allfile(arrentri,boot,addata,chose);
                  state=INFILE;
               }
               //folder
               if (arrentri[chose-1].attr==0x10)
               {
                  printf("\033[H\033[J");
                  // if previus folder is root
                  if (arrentri[chose-1].firstCluster==0)
                  {
                     adroot= (boot.num_fat*boot.blocks_per_fat+1)*boot.bytes_per_block;
                     readRoot(arrentri,boot.num_root_dir_entries,adroot,&lenentri);
                  }else
                  {
                     adroot= addata+arrentri[chose-1].firstCluster*boot.bytes_per_block;
                     readFolder(arrentri,boot.bytes_per_block/32,adroot,&lenentri);
                  }
                  print_allentri(lenentri,arrentri);
               }
            }
            // stop the program
            if (chose==0)
            {
               state=CLOSEE;
            }
            break;
         case INFILE:
            if (chose==0)
            {
               printf("\033[H\033[J");
               print_allentri(lenentri,arrentri);
               state=INFOLDER;
            }
            break;
         default:
            break;
      }
      print_control_instructions(state);
      
   } 
}