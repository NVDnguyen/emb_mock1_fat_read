#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
#include "fat_data_reader.h"
#include "console_utils.h"

/* Function to print directory entry information, highlighting directories in blue. */

int main()
{
   /* Open the file system image for reading */
   FILE *f = fopen(FILE_PATH, "rb");
   status_t status;
   BootBlock boot;

   /* Read the boot block to obtain metadata information */
   status = readBootBlock(&boot, f);
   //printBootBlock(&boot);
   if (status == OK)
   {
      uint8_t flag = 0;

      while (flag != 1)
      {
         /* Read root directory entries */
         DirectoryEntry dirEntries[boot.num_root_dir_entries];
         uint8_t numEntry = readRootEntry(dirEntries, f, boot);

         if (numEntry != 0)
         {
            /* Print header for directory entries */
            printDirectoryEntries2(dirEntries, numEntry);

            printFooter();

            uint8_t choice;
            while (1)
            {
               printf("\n> Select to open (1 to %d): ", numEntry);

               /* Get user input and validate it */
               if (scanf("%hhu", &choice) != 1)
               {
             
                  printf("\n> Input not between 1 and %d.\n", numEntry);
                  while (getchar() != '\n')
                     ; /* Clear invalid input */
                  continue;
                  
               }

               /* Check for valid entry choice */
               if (choice < 0 || choice > numEntry)
               {
                  setColor(COLOR_RED, COLOR_BLACK);
                  printf("Please select a valid entry.\n");
                  setColor(COLOR_WHITE,COLOR_BLACK);
               }
               /* Option to clear the console and redisplay entries */
               else if (choice == 0)
               {
                  clearConsole();
                  /*show*/
                  printDirectoryEntries2(dirEntries, numEntry);
                  printFooter();
               }
               else
               {
                  /* Handle directory or file entry selection */
                  DirectoryEntry selectedEntry = dirEntries[choice - 1];
                  if (selectedEntry.attributes == FILE_ATTR_DIRECTORY)
                  {
                     printf("\n======== Child Entries ========\n");
                     printf("\n[%02X]\n",selectedEntry.startingCluster);
                     flag = 1;
                     break;
                  }
                  /* Display file content if a regular file is selected */
                  else if (selectedEntry.attributes == NONE)
                  {
                     setColor(COLOR_LIGHT_GREEN, COLOR_BLACK);
                     printf("\n======== %s ========\n", selectedEntry.filename);
                     setColor(COLOR_BLACK, COLOR_WHITE);
                     displayDataInFile(selectedEntry.startingCluster, f, boot);   
                     setColor(COLOR_DEFAULT, COLOR_BLACK);
                  }
               }
            }
         }
         else
         {
            setColor(COLOR_RED, COLOR_BLACK);
            printf("\n<ERROR>\n");
            setColor(COLOR_WHITE, COLOR_BLACK);
            
         }
      }
   }

   /* Close the file after operations are complete */
   fclose(f);
   return 0;
}
