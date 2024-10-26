#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
#include "fat_data_reader.h"
#include "console_utils.h"

void print_control_instructions(State_t a)
{
   printf("\n\n");
   if (a == INFOLDER)
   {
      printf("0: Stop program\n\n");
   }
   else if (a == INFILE)
   {
      printf("0: Exit file\n\n");
   }
   printf("\n>  ");
}

int main()
{
   FILE *f = fopen(FILE_PATH, "rb");
   if (f == NULL)
   {
      perror("Failed to open the file");
      return EXIT_FAILURE;
   }

   status_t status;
   BootBlock boot;
   status = readBootBlock(&boot, f);
   if (status != OK)
   {
      fprintf(stderr, "Error reading boot block.\n");
      fclose(f);
      return EXIT_FAILURE;
   }

   uint32_t addata = (boot.blocks_per_fat * boot.num_fat +
                      boot.num_root_dir_entries * 32 / boot.bytes_per_block - 1) *
                     boot.bytes_per_block;
   uint32_t adroot = (boot.num_fat * boot.blocks_per_fat + 1) * boot.bytes_per_block;

   State_t state = INFOLDER;
   DirectoryEntry dirEntries[boot.num_root_dir_entries];
   uint8_t numEntry = readRootEntry(f, dirEntries, boot.num_root_dir_entries, adroot);
   print_allentri(dirEntries, numEntry);
   printFooter();

   uint8_t choice;
   print_control_instructions(state);

   while (state != EXIT)
   {
      scanf("%d", &choice);
      printf("<%d-%d>",choice,numEntry);
      switch (state)
      {
      case INFOLDER:
         printf("INFOLDER");
         if (choice == 0)
         {
            state = EXIT;
         }
         else if (choice > 0 && choice <= numEntry)
         {
            
            if (dirEntries[choice - 1].attributes == 0x0)
            { // File
               printf("\033[H\033[J");
               setColor(COLOR_GREEN,COLOR_BLACK);
               displayDataInFile(dirEntries[choice - 1].startingCluster, f, boot);
               setColor(COLOR_WHITE,COLOR_BLACK);
               state = INFILE;
            }
            else if (dirEntries[choice - 1].attributes == 0x10)
            { // Folder
               printf("\033[H\033[J");
               if (dirEntries[choice - 1].startingCluster == 0)
               {
                  adroot = (boot.num_fat * boot.blocks_per_fat + 1) * boot.bytes_per_block;
                  numEntry =readRootEntry(f, dirEntries, boot.num_root_dir_entries, adroot);
               }
               else
               {
                  adroot = addata + dirEntries[choice - 1].startingCluster * boot.bytes_per_block;
                  numEntry= readFolder(f, dirEntries, boot.bytes_per_block / 32, adroot);
               }
               print_allentri(dirEntries, numEntry);
            }
         }
         break;

      case INFILE:
         if (choice == 0)
         {
            printf("\033[H\033[J");
            print_allentri(dirEntries, numEntry);
            state = INFOLDER;
         }
         break;

      default:
         printf("Invalid state.\n");
         state = EXIT;
         break;
      }

      print_control_instructions(state);
   }

   fclose(f);
   return 0;
}
