/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Scheduler implementation.                                                 */
/*****************************************************************************/

#ifndef __schedule_include__
#define __schedule_include__

#include "thread/dispatch.h"
#include "object/queue.h"
#include "thread/entrant.h"

class IdleThread : public Entrant
{
public:
	IdleThread(const IdleThread &copy) = delete; // prevent copying
	IdleThread(void* tos);

	void action();
};

class Scheduler :
public Dispatcher
{
private:
	IdleThread idleThread;

protected:
	Queue readyList;

	void nothingHandler();

public:
	Scheduler (const Scheduler &copy) = delete; // prevent copying
	Scheduler();

	void ready(Entrant& that);
	void schedule();
	void exit();
	void kill(Entrant& that);
	void resume();
 
};

#endif
