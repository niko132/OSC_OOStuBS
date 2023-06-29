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

// Qemu works with a lower timer interval
// (20us seems ok)
// but qemu has no exact time its delayed by a random amount

// Real system needs to be tested
Watch watch(1000); // in emulator a value between 100 - 1000 seems ok
