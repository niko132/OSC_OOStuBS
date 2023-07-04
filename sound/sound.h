/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                S O U N D                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Sound output.                                                             */
/*****************************************************************************/

#ifndef __sound_include__
#define __sound_include__

#include "machine/io_port.h"

class Sound {
private:
	const IO_Port ctrl_port; // control register (W)
	const IO_Port channel2_port; // output (R) and input (W) of channel0 timer
    const IO_Port kc_port; // keyboard controller port

	const int TIMER_FREQ = 1193182; // ~1.2MHz

    unsigned long long currentIndex = 0;

public:
	Sound(const Sound &copy) = delete; // prevent copying
	Sound() : ctrl_port(0x43), channel2_port(0x42), kc_port(0x61) { }

    void play_sound(unsigned int frequency);

    void attach_to_timer();

    void play_buffer();
};

extern Sound sound;

extern unsigned int test_freq;

#endif
