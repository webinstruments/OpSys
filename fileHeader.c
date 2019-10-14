//
// Created by root on 10/14/19.
//

#include <unistd.h>
#include <stdio.h>
#include "fileHeader.h"

int setField(int fd, uint32_t* field) {
    int result = read(fd, field, sizeof(uint32_t));
    if(result == -1) {
        perror("main");
    }
    return result;
}

FileHeader getHeaderInformation(int fd) {
    FileHeader result;
    setField(fd, &result.type);
    setField(fd, &result.offset);
    setField(fd, &result.length);
    setField(fd, &result.encoding);
    setField(fd, &result.samplingRate);
    setField(fd, &result.audioChannels);

    return result;
}

