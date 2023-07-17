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

class SB16 : public Gate {
private:
    unsigned char* DMA_BUFFER = (unsigned char*)0x010000;
    unsigned char buffer[20 * 8 * 1024]; // TODO: find good buffer size
    unsigned int writeIndex, readIndex;

public:
	SB16(const SB16 &copy) = delete; // prevent copying
	SB16() : writeIndex(0), readIndex(0) {
        // zero the audio buffer
        for (int i = 0; i < 8 * 1024; i++) {
            // DMA_BUFFER[i] = 0; // maybe with 127 ???

            int val = i / 4;
            DMA_BUFFER[i] = val % 2 == 0 ? 0 : 255; // generate a square signal
        }

        // TODO; zero buffer
    }

    inline void reset() {
        reset_sb();
    }

    inline void programm() {
        programm_sb();
    }

    void windup();

    // PROLOGUE: Contains the prologue of the interrupt handler.
	bool prologue();

	// EPILOGUE: This method triggers the process switch.
	void epilogue();

    void addSamples(unsigned char* sampleBuf, unsigned int bytes);
};

extern SB16 sb16;

#endif
