/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         C U S T O M E R                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Customer class extends the Entrant class with the ability to record   */
/* and retrieve an event that the process in question is waiting for.        */
/*****************************************************************************/

#ifndef __customer_include__
#define __customer_include__

#include "thread/entrant.h"
#include "meeting/waitingroom.h"

class Customer : public Entrant
{
private:
	Waitingroom* waitingroom = nullptr;
public:
	Customer (const Customer &copy) = delete; // prevent copying
	Customer(void* tos) : Entrant(tos), waitingroom(nullptr) { };

	void waiting_in(Waitingroom *w) {
		this->waitingroom = w;
	};

	Waitingroom* waiting_in() {
		return this->waitingroom;
	};
};

#endif
