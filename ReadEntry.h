#ifndef __READENTRY_H__
#define __READENTRY_H__


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FILENAME_LENGTH 8
#define EXTENSION_LENGTH 3
#define RESERVED_LENGTH 10 

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
typedef struct {
	
	char fileName[FILENAME_LENGTH];
	char fileExtention[EXTENSION_LENGTH];
	uint8_t attributes;
	uint8_t reserved[RESERVED_LENGTH];
	timeCreat;
	dateCreat;
	uint32_t firstCluster;
	uint32_t fileSize;
};



#endif 