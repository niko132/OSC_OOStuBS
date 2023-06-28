/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              W A T C H                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Handles timer interrupts by managing a time slice and triggering a        */
/* process switch if necessary.                                              */
/*****************************************************************************/

#include "device/watch.h"
#include "machine/plugbox.h"
#include "machine/pic.h"
#include "syscall/guarded_organizer.h"
#include "meeting/bellringer.h"

void Watch::windup() {
    plugbox.assign(plugbox.timer, *this);
    pic.allow(PIC::timer);
}

bool Watch::prologue() {
    return true; // always run the epilogue when the timer fires an interrupt
}

void Watch::epilogue() {
    bellringer.check();
    organizer.Organizer::resume();
}

// VirtualBox needs a rather high timer value
// (1000us)
// otherwise the threads/epilogues can't catch up and just get queued
// while more and more interrupts get executed
// CAUTION: VirtualBox seems to slow the timer down by 10x!!!

// VMWare handles the timers correct
// Use VMWare rather than VB!!!

// Qemu works with a lower timer interval
// (20us seems ok)
// but qemu seems to ignore the concrete value and just fire the timer randomly
// therefore no accurate time measurement is possible

// Real system needs to be tested
Watch watch(1000);
