#include <stdint.h>
#include <stdio.h>
#include "fat_boot_reader.h"

int main()
{
   status_t status = readBootBlock() ;
   printf("\n%s\n",status_strings[status]);

}