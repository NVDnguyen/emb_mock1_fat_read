// #include <stdio.h>
// #include <stdint.h> 
// #include "ParseData.h"

// FILE *fileptr = NULL;

// BootBlock_t bootblock;


// typedef enum OpenFileFatStatus {
// 	OPEN_SUCCESS,
// 	OPEN_FAIL
// } OpenFileFatStatus_t;

// OpenFileFatStatus_t OpenFileFat(); 
// OpenFileFatStatus_t CloseFileFat(); 


// int main() {
// 	//fileptr = fopen("floppy.img", "r");
// 	uint8_t exit = 1;
// 	StatusReadBoot_t statusreadboot;
// 	OpenFileFatStatus_t OpenFileStatus; 
// 	OpenFileStatus = OpenFileFat(); 
// 	switch (OpenFileStatus) {
// 		case OPEN_SUCCESS: 
// 			//Read Root Block
// 			statusreadboot = ReadBoot(&bootblock);
// 			switch (statusreadboot) {
// 				case READBOOT_SUCCESS:
// 					//printBootBlock(&bootblock);		
// 					// Print Root Directory 
// 					//
// 						DisplayData(0x2EB);
// 					//
// 					CloseFileFat();
// 					break;
// 				case READBOOT_FAIL: 
// 					CloseFileFat();
// 					break;
// 			}
// 			break;
// 		case OPEN_FAIL:
// 			printf("OPEN FAIL \n");
// 			break;
// 	}
// 	return 0;
// }


// OpenFileFatStatus_t OpenFileFat() {
// 	OpenFileFatStatus_t status = OPEN_SUCCESS;
// 	fileptr = fopen("floppy.img", "r + b");
// 	if (fileptr != NULL) {
		
// 		//None
// 	}		
// 	else {
// 		status = OPEN_FAIL;
// 	} 
	
// 	return status;
// }


// OpenFileFatStatus_t CloseFileFat() {
// 	OpenFileFatStatus_t status = OPEN_SUCCESS;
// 	fclose(fileptr);
// 	if (fileptr == NULL) {
// 		status = OPEN_FAIL;
// 	}
// 	else {
// 		// None
// 	}
	
// 	return status;
// }