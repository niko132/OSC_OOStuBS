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

#include "syscall/guarded_organizer.h"

bool Panic::prologue() {
    kout.flush();
    kout << "Stopping CPU!" << endl;
    kout << "Stack content:" << endl;
    kout << endl << endl;

    unsigned char* sp = (unsigned char*)organizer.active()->toc.rsp;
    for (int i = 0; i < 80; i++) {
        if (i % 8 == 0) kout << endl;
        unsigned int val = (unsigned int)sp[i]; // stack grows upwards
        if (val < 16) kout << "0";
        kout << hex << val << " ";
    }
    kout.flush();

    cpu.halt();

    return false; // not executed but still needed
}

Panic panic; 