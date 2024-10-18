#include <stdint.h>
#include <stdio.h>
#include "fat_boot_reader.h"

int main()
{
   if(readBootBlock() == OK){
      printf("___ok____\n");
   }else{
      printf("___error____\n");
   }
   
}