/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                    G U A R D E D _ O R G A N I Z E R                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Guarded_Organizer implements the system call interface to the         */
/* Organizer.                                                                */
/*****************************************************************************/

#include "syscall/guarded_organizer.h"
#include "guard/secure.h"

void Guarded_Organizer::ready(Thread& that) {
    Secure secure;
    Organizer::ready(that);
}

void Guarded_Organizer::exit() {
    Secure secure;
    Organizer::exit();
}

void Guarded_Organizer::kill(Thread& that) {
    Secure Secure;
    Organizer::kill(that);
}

void Guarded_Organizer::resume() {
    Secure Secure;
    Organizer::resume();
}

Guarded_Organizer organizer;

