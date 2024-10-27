#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat_boot_reader.h"
#include "fat_root_reader.h"
#include "fat_data_explorer.h"
#include "console_utils.h"

void sanitizeFilename(char *raw_name, char *clean_name, int length);

int main()
{
   /* Open the file system image in binary read mode */
   FILE *f = fopen(FILE_PATH, "rb+");
   if (f == NULL)
   {
      notifyError("\nFailed to open the file.\n");
      return EXIT_FAILURE;
   }

   /* Read boot sector and validate */
   status_t status;
   BootBlock boot;
   status = readBootBlock(&boot, f);
   if (status != OK)
   {
      notifyError("\nError reading boot block.\n");
      fclose(f);
      return EXIT_FAILURE;
   }

   /* Verify that all copies of the FAT table are identical */
   if (verifyFATTable(&boot, f) == OK)
   {
      notifySuccess("\nAll FAT tables are consistent.\n");
   }
   else
   {
      notifyError("\nFAT tables are inconsistent.\n");
   }

   /* Calculate addresses for data and root directory based on FAT configuration */
   uint32_t addata = (boot.blocks_per_fat * boot.num_fat +
                      boot.num_root_dir_entries * 32 / boot.bytes_per_block - 1) *
                     boot.bytes_per_block;
   uint32_t adroot = (boot.num_fat * boot.blocks_per_fat + 1) * boot.bytes_per_block;

   /* Initialize the state to browse the root folder */
   State_t state = INFOLDER;
   char current_path[256] = "~";

   /* Load and print entries in the root folder */
   DirectoryEntry dirEntries[boot.num_root_dir_entries];
   uint8_t numEntry = readRootEntry(f, dirEntries, boot.num_root_dir_entries, adroot);
   print_allentri(dirEntries, numEntry);
   printFooter();

   /* Display initial control instructions */
   char command[100];

   while (state != EXIT)
   {
      showPrompt("root@fsoft-mygroup:");
      printf("%s$ ", current_path);
      fgets(command, sizeof(command), stdin);

      /* =========================================== cd ==============================================*/
      if (strncmp(command, "cd ", 3) == 0 || strncmp(command, "rm ", 3) == 0)
      {
         char *dir_name = command + 3;
         dir_name[strcspn(dir_name, "\n")] = 0;
         int found = -1;
         for (int i = 0; i < numEntry; i++)
         {
            char temp_name[9];
            sanitizeFilename(dirEntries[i].filename, temp_name, 8);
            if (strcmp(temp_name, dir_name) == 0)
            {
               found = i;
               break;
            }
         }

         if (found != -1)
         {
            if (strncmp(command, "rm ", 3) == 0)
            {
               if (removeFile(dirEntries[found], adroot, found, f) == OK)
               {
                  notifySuccess("\nRemove file successfully\n");
               }
               else
               {
                  notifyError("\nFailed to remove this file.\n");
               }
            }
            else
            {
               if (dirEntries[found].attributes == 0x0)
               {
                  clearConsole();
                  setColor(GREEN, BLACK);
                  displayDataInFile(dirEntries[found].startingCluster, f, boot, dirEntries[found].fileSize);
                  setColor(WHITE, BLACK);
                  printf("\n>'back' to exit file\n");
               }
               else if (dirEntries[found].attributes == 0x10)
               {
                  if (dirEntries[found].startingCluster == 0)
                  {
                     adroot = (boot.num_fat * boot.blocks_per_fat + 1) * boot.bytes_per_block;
                     numEntry = readRootEntry(f, dirEntries, boot.num_root_dir_entries, adroot);
                     strcpy(current_path, "~");
                  }
                  else
                  {

                     adroot = addata + dirEntries[found].startingCluster * boot.bytes_per_block;
                     numEntry = readFolder(f, dirEntries, boot.bytes_per_block / 32, adroot);
                     /*update current path*/
                     char temp_name[9];
                     sanitizeFilename(dirEntries[found].filename, temp_name, 8);
                     if (strcmp(temp_name, "..") == 0)
                     {
                        char *lastSlash = strrchr(current_path, '/');
                        if (lastSlash != NULL && lastSlash != current_path)
                        {
                           *lastSlash = '\0';
                           strcat(current_path, "/");
                        }
                        else
                        {
                           strcpy(current_path, "/");
                        }
                     }
                     else if (strcmp(temp_name, ".") != 0)
                     {
                        strcat(current_path, "/");
                        strcat(current_path, temp_name);
                     }
                  }
                  clearConsole();
                  print_allentri(dirEntries, numEntry);
               }
            }
         }
         else
         {
            printf("Folder not found: %s\n", dir_name);
         }
      }
      else if (strcmp(command, "clean\n") == 0 || strcmp(command, "back\n") == 0)
      {
         clearConsole();
         print_allentri(dirEntries, numEntry);
      }
      else if (strcmp(command, "exit\n") == 0)
      {
         state = EXIT;
      }
   }
   fclose(f);
   return 0;
}

void sanitizeFilename(char *raw_name, char *clean_name, int length)
{
   strncpy(clean_name, raw_name, length);
   clean_name[length] = '\0';
   for (int i = length - 1; i >= 0 && clean_name[i] == ' '; i--)
   {
      clean_name[i] = '\0';
   }
}