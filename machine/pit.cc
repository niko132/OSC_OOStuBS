/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  P I T                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Programmable Interval Timer.                                              */
/*****************************************************************************/

#include "machine/pit.h"

void PIT::interval(int us) {
    short ticks = us * TIMER_FREQ; // maybe round the result
    char* ticksBytes = (char*)&ticks;

    ctrl_port.outb(0x34); // periodic interrupts on channel0
    channel0_port.outb(ticksBytes[1]);
    channel0_port.outb(ticksBytes[0]);

    interval_us = us;
}

