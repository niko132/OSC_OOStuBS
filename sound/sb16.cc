#include "sound/sb16.h"

#include "machine/pic.h"
#include "machine/plugbox.h"

#include "object/memory.h"

void SB16::windup() {
    plugbox.assign(plugbox.sb16, *this);
    pic.allow(PIC::sb16);
}

bool SB16::prologue() {
    return true;
}

void SB16::epilogue() {
    copyBuffer();
    if (readIndex >= sampleBytes) readIndex = 0;
    reset_sb();
    // if (shouldStop) return;
    
    single_mode_sb();
}

void SB16::copyBuffer() {
    memcpy(DMA_BUFFER, &buffer[readIndex], DMA_BUFFER_SIZE);
    readIndex += DMA_BUFFER_SIZE;
}

void SB16::addSamples(unsigned char* sampleBuf, unsigned int bytes) {
    if (writeIndex + bytes < sizeof(buffer)) {
        // single transfer
        memcpy(&buffer[writeIndex], sampleBuf, bytes);
        writeIndex += bytes;
    } else {
        // two transfers
        unsigned int firstBytes = sizeof(buffer) - writeIndex;
        unsigned int secondBytes = bytes - firstBytes;
        memcpy(&buffer[writeIndex], sampleBuf, firstBytes);
        memcpy(&buffer[0], &sampleBuf[firstBytes], secondBytes);
        writeIndex = secondBytes;
    }

    sampleBytes += bytes;
}

SB16 sb16;
