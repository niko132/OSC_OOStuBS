/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 P A N I C                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Default interrupt handler.                                                */
/*****************************************************************************/
#include "device/panic.h"
#include "machine/cpu.h"
#include "device/cgastr.h"

bool Panic::prologue() {
    kout << "Stopping CPU!" << endl; 
    cpu.halt();
    return false; // not executed but still needed
}

Panic panic; 