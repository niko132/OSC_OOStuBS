/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                   G U A R D E D _ O R G A N I Z E R                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Guarded_Organizer implements the system call interface to the         */
/* Organizer.                                                                */
/*****************************************************************************/

#ifndef __Guarded_Organizer_include__
#define __Guarded_Organizer_include__

#include "syscall/thread.h"
#include "thread/organizer.h"

class Guarded_Organizer : public Organizer {
public:
	Guarded_Organizer() {}
	Guarded_Organizer(const Guarded_Organizer &copy) = delete; // prevent copying

	void ready(Thread& that);
	void exit();
	void kill(Thread& that);
	void resume();
};

extern Guarded_Organizer organizer;

#endif
