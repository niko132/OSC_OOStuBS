/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  P I T                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Programmable Interval Timer.                                              */
/*****************************************************************************/

#ifndef __pit_include__
#define __pit_include__

#include "machine/io_port.h"

class PIT {
private:
	int interval_us;

	const IO_Port ctrl_port; // control register (W)
	const IO_Port channel0_port; // output (R) and input (W) of channel0 timer

	const int TIMER_FREQ = 1193182; // ~1.2MHz

public:
	PIT(const PIT &copy) = delete; // prevent copying
	PIT(int us) : ctrl_port(0x43), channel0_port(0x40) {
		interval (us);
	}
	int interval() {
		return interval_us;
	}
	void interval(int us);
};

#endif
