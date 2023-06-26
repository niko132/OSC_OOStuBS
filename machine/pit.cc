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

    ctrl_port.outb(0x34); // periodic interrupts on channel0

    // low byte then high byte
    // let the outb() function handle the masking
    channel0_port.outb(ticks);
    channel0_port.outb(ticks >> 8);

    interval_us = us;
}

