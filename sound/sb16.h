/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                S O U N D                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Sound output.                                                             */
/*****************************************************************************/

#ifndef __sb16_include__
#define __sb16_include__

#include "guard/gate.h"

extern "C" void reset_sb();
extern "C" void programm_sb();
extern "C" void ack_int_sb();
extern "C" void single_mode_sb();
extern "C" void make_int();

#define DMA_BUFFER_SIZE 8192

class SB16 : public Gate {
private:
    unsigned char* DMA_BUFFER = (unsigned char*)0x010000;
    unsigned char buffer[20 * 22050]; // 20s of 22050Hz audio
    unsigned int writeIndex, readIndex;
    unsigned int sampleBytes;
    bool shouldStop;

public:
	SB16(const SB16 &copy) = delete; // prevent copying
	SB16() : writeIndex(0), readIndex(0) {
        // zero the audio buffer
        for (int i = 0; i < DMA_BUFFER_SIZE; i++) {
            DMA_BUFFER[i] = 0;
        }

        // zero buffer
        for (int i = 0; i < sizeof(buffer); i++) {
            buffer[i] = 0;
        }
        sampleBytes = 0;
        shouldStop = true;
    }

    inline void reset() {
        reset_sb();
    }

    inline void programm() {
        copyBuffer(); // fill the buffer
        shouldStop = false;
        programm_sb();
    }

    void windup();

    // PROLOGUE: Contains the prologue of the interrupt handler.
	bool prologue();

	// EPILOGUE: This method triggers the process switch.
	void epilogue();

    void addSamples(unsigned char* sampleBuf, unsigned int bytes);

    void copyBuffer();

    inline void stop() {
        shouldStop = true;
    }

    inline void resetBuffer() {
        writeIndex = 0;
        readIndex = 0;

        // zero the audio buffer
        for (int i = 0; i < DMA_BUFFER_SIZE; i++) {
            DMA_BUFFER[i] = 0;
        }

        // zero buffer
        for (int i = 0; i < sizeof(buffer); i++) {
            buffer[i] = 0;
        }
        sampleBytes = 0;
        shouldStop = true;
    }
};

extern SB16 sb16;

#endif
