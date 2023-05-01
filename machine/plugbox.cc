/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              P L U G B O X                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Abstracts an interrupt vector table. Allows to configure handler routines */
/* for every hardware and software interrupt and every CPU exception.        */
/*****************************************************************************/

#include "machine/plugbox.h"
#include "device/panic.h"


Plugbox::Plugbox(){
    // init gates with panic for minimal interrupt handling
    for(int i = 0; i < NUM_GATES; i++)
        gates[i] = &panic; 
}

void Plugbox::assign(unsigned int slot, Gate& gate){
    gates[slot] = &gate; 
}

Gate& Plugbox::report (unsigned int slot){
    return *gates[slot]; 
}

