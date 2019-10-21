#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <byteswap.h>
#include "fileHeader.h"

int printError() { // test
    perror("main");
    return 0;
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

int fileInput() {
    int result = -1;
    char str[100];
    do {
        printf("Please enter input path...\n");
        scanf("%s", str);
        printf("%s\n", str);
        result = open(str, O_RDONLY);
        perror("main");
    } while (result == -1);
    return result;
}

int main() {
    int fdInput = fileInput();
    printf("Reading file...\n");
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
        printError();
    }

    printf("Reading file backwards...\n");
    uint8_t * buffer = readFileBackwards(headerLength, fdInput);
    if (buffer == NULL) {
        printError();
    }

    printf("Writing result...\n");
    fdResult = write(fdOutput, buffer, sizeof(uint8_t) * headerLength);
    if (fdResult == -1) {
        return printError();
    }

    free(buffer);

    return 0;
}