#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#define FILE_PATH "data/floppy.img"
#define SIZE_BLOCK 512
typedef enum
{
    ERROR,
    OK,
    ERROR_NULL_FILE,
    ERROR_READ,
    ERROR_MEMORY_ALLOCATION
} status_t;

static const char* status_strings[] = {
    "ERROR",
    "OK",
    "ERROR_NULL_FILE",
    "ERROR_READ",
    "ERROR_MEMORY_ALLOCATION"
};
#endif /*STATUS_CODES_H*/ 
