#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fat_readdata.h"

status_t print_data(uint16_t address, uint16_t max) {
    status_t status = OK;
    FILE *f = fopen(FILE_PATH, "rb");  
    // Kiểm tra nếu file mở thành công
    if (f == NULL) {
        printf("Không thể mở file.\n");
        return ERROR;
    }
    
    // Di chuyển con trỏ file đến địa chỉ yêu cầu
    fseek(f, address, SEEK_SET);
    
    for (int i = 0; i < max; i++) {
        int ch = fgetc(f);
        
        // Kiểm tra nếu đã đạt đến EOF
        if (ch == EOF) {
            break;  // Dừng lại nếu không còn dữ liệu
        }
        
        // In ra ký tự
        printf("%c", (char)ch);
    }
    
    fclose(f);  // Đóng file
    return status;  // Trả về trạng thái thành công
}
