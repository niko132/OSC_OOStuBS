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
    short ticks = us * TIMER_FREQ / 1000000; // maybe round the result

    ctrl_port.outb(0x34); // periodic interrupts on channel0

    // low byte then high byte
    // let the outb() function handle the masking
    unsigned char lowByte = ticks & 0xff;
    unsigned char highByte = (ticks >> 8) & 0xff;

    channel0_port.outb(lowByte);
    channel0_port.outb(highByte);

    interval_us = us;
}

