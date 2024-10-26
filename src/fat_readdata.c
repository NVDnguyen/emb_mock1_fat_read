#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fat_readdata.h"
#include "fat_reader.h"
status_t print_data(uint16_t address, uint16_t max) {
    status_t status = OK;
    FILE *f = fopen(FILE_PATH, "rb");  
    // Kiểm tra nếu file mở thành công
    if (f == NULL) {
        printf("Không thể mở file.\n");
        return ERROR;
    }
    
    fseek(f, address, SEEK_SET);
    
    for (int i = 0; i < max; i++) {
        int ch = fgetc(f);
        

        if (ch == EOF) {
            printf("\n");
            break;  
        }
        
 
        printf("%c", (char)ch);
    }
    
    fclose(f);  
    return status;  
}