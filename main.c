#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <byteswap.h>
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

uint8_t* readFileBackwards(uint32_t size, int sourceFd) {
    uint8_t* buffer = malloc(size * sizeof(uint8_t));
    uint8_t sample;
    for(int i = 1; i <= size; ++i) {
        lseek(sourceFd, i * -1, SEEK_END);
        if(read(sourceFd, &sample, sizeof(uint8_t)) == -1) {
            perror("main");
            return NULL;
        }
        buffer[i - 1] = sample;
    }
    return buffer;
}

int main() {
    printf("Reading file...\n");
    int fdInput = open("input.au", O_RDONLY);
    int fdOutput = open("output.au", O_WRONLY|O_TRUNC|O_CREAT|O_APPEND, 0666);

    FileHeader header = getHeaderInformation(fdInput);
    uint32_t headerLength = bswap_32(header.length);
    printf("offset: 0x%08x\n", bswap_32(header.offset));
    printf("encoding: 0x%08x\n", bswap_32(header.encoding));
    printf("length: 0x%08x\n", bswap_32(headerLength));
    printf("rate: 0x%08x\n", bswap_32(header.samplingRate));
    printf("channels: 0x%08x\n", bswap_32(header.audioChannels));

    uint32_t arr[] = { header.type, header.offset, header.length, header.encoding, header.samplingRate, header.audioChannels };
    int fdResult = write(fdOutput, arr, sizeof(uint32_t) * 6);
    if (fdResult == -1) {
        perror("main");
        return 0;
    }

    printf("Reading file backwards...\n");
    int32_t* buffer = readFileBackwards(headerLength, fdInput);
    if (buffer == NULL) {
        printf("Something went wrong");
        return 0;
    }

    printf("Write result\n");
    fdResult = write(fdOutput, buffer, sizeof(uint8_t) * headerLength);
    if (fdResult == -1) {
        perror("main");
        return 0;
    }

    return 0;
}