// status_codes.h
#ifndef STATUS_CODES_H
#define STATUS_CODES_H

typedef enum
{
    ERROR,
    OK,
    FILE_NULL_ERROR,
    READ_ERROR
} status_t;

static const char* status_strings[] = {
    "ERROR",
    "OK",
    "FILE_NULL_ERROR",
    "READ_ERROR"
};
#endif // STATUS_CODES_H
