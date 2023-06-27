/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           S E M A P H O R E                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The class Semaphore implements the synchronization concept of the         */
/* counting semaphore.                                                       */
/*****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__

#include "meeting/waitingroom.h"

class Semaphore : public Waitingroom
{
private:
	int c;

public:
	Semaphore(const Semaphore &copy) = delete; // prevent copying
	Semaphore(int c);

	void p();
	void v();

	void wait() {
		p();
	};

	void signal() {
		v();
	};
};

#endif
