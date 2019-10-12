//
// Created by root on 10/12/19.
//

#ifndef READMUSIC_FILEHEADER_H
#define READMUSIC_FILEHEADER_H

#include <stdint.h>

typedef struct FileHeader {
    uint32_t type;
    uint32_t offset;
    uint32_t length;
    uint32_t encoding;
    uint32_t samplingRate;
    uint32_t audioChannels;
} FileHeader;

#endif //READMUSIC_FILEHEADER_H
