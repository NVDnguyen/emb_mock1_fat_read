#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#define PATH_FILE "data/floppy.img"

typedef enum
{
    ERROR,
    OK,
    FILE_NULL_ERROR,
    READ_ERROR,
    MEMORY_ALLOCATION_ERROR
} status_t;

static const char* status_strings[] = {
    "ERROR",
    "OK",
    "FILE_NULL_ERROR",
    "READ_ERROR",
    "MEMORY_ALLOCATION_ERROR"
};
#endif /*STATUS_CODES_H*/ 
