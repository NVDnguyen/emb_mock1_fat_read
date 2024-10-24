#ifndef STATUS_CODES_H
#define STATUS_CODES_H
#define FILE_PATH "data/floppy.img"
typedef enum
{
    ERROR,
    OK,
    ERROR_NULL_FILE,
    ERROR_READ,
    ERROR_MEMORY_ALLOCATION,
    ERROR_UNSUPPORT_FILE_SYSTEM
} status_t;

static const char* status_strings[] = {
    "ERROR",
    "OK",
    "ERROR_NULL_FILE",
    "ERROR_READ",
    "ERROR_MEMORY_ALLOCATION",
    "ERROR_UNSUPPORT_FILE_SYSTEM"
};
#endif /*STATUS_CODES_H*/ 
