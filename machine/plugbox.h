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

#ifndef __Plugbox_include__
#define __Plugbox_include__

#include "guard/gate.h"

#define NUM_GATES 64

class Plugbox {
public:
	Plugbox(); 
	Plugbox(const Plugbox &copy) = delete; // prevent copying

	const unsigned short timer = 32; 
	const unsigned short keyboard = 33; 

	void assign (unsigned int slot, Gate& gate); 
	Gate& report (unsigned int slot);

private:
	Gate * gates[NUM_GATES];
};

#endif
