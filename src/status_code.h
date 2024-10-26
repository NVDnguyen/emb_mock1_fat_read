#ifndef STATUS_CODES_H
#define STATUS_CODES_H
#define FILE_PATH "data/floppy.img"
#define FILE_PATH1 "data/floppy1.img"
typedef enum
{
    ERROR,
    OK,
    ERROR_NULL_FILE,
    ERROR_READ,
    ERROR_MEMORY_ALLOCATION,
    ERROR_UNSUPPORT_FILE_SYSTEM,
    OK_FAT,
    ERROR_FAT
} status_t;
typedef enum State_t
{
   START,
   BACK,
   INFOLDER,
   INFILE,
   PROCESSING,
   EXIT,
   REGET   
} State_t;

static const char* status_strings[] = {
    "ERROR",
    "OK",
    "ERROR_NULL_FILE",
    "ERROR_READ",
    "ERROR_MEMORY_ALLOCATION",
    "ERROR_UNSUPPORT_FILE_SYSTEM"
};
typedef enum
{
    NONE = 0x00,
    FILE_ATTR_READ_ONLY = 0x01,
    FILE_ATTR_HIDDEN = 0x02,
    FILE_ATTR_SYSTEM = 0x04,
    FILE_ATTR_VOLUME_LABEL = 0x08,
    FILE_ATTR_DIRECTORY = 0x10,
    FILE_ATTR_ARCHIVE = 0x20,
    FILE_ATTR_UNUSED_1 = 0x40,
    FILE_ATTR_UNUSED_2 = 0x80
} FileAttributes;

static const char* attribute_strings[] = {
    "NONE",
    "READ ONLY",
    "HIDDEN",
    "SYSTEM",
    "VOLUME LABEL",
    "DIR",
    "ARCHIVE",
    "UNUSED 1",
    "UNUSED 2"
};


#endif /*STATUS_CODES_H*/ 
