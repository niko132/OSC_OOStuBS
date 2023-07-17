#include "sound/sb16.h"

#include "machine/pic.h"
#include "machine/plugbox.h"

#include "object/memory.h"
#include "sound/pcm_sample.h"

#include "device/cgastr.h"
#include "machine/cpu.h"

void SB16::windup() {
    plugbox.assign(plugbox.sb16, *this);
    pic.allow(PIC::sb16);
}

unsigned long long index = 0;

bool SB16::prologue() {
    return true;
}

void SB16::epilogue() {
    /*
    memcpy(DMA_BUFFER, (void*)&PCM_SAMPLE_1[index], 8 * 1024);
    index += 8 * 1024;
    if (index >= sizeof(PCM_SAMPLE_1)) index = 0;
    */

    memcpy(DMA_BUFFER, &buffer[readIndex], 8 * 1024);
    readIndex += 8 * 1024;
    if (readIndex >= sizeof(buffer)) readIndex = 0;

    reset_sb();
    single_mode_sb();
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
}

SB16 sb16;
