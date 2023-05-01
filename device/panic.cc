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

void Panic::trigger() {
    kout << "Stopping CPU!"; 
    cpu.halt(); 
}

Panic panic; 