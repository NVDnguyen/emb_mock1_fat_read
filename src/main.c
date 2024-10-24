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
   //printBootBlock(&boot);
 
   DirectoryEntry *dirEntries = NULL;
   status = readDirectoryEntry(&dirEntries,boot);
   printf("\n%s\n",status_strings[status]);
   printDirectoryEntries(dirEntries,boot.num_root_dir_entries);

   

}