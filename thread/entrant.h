/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            E N T R A N T                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* A coroutine that is managed by the Scheduler.                             */
/*****************************************************************************/

#ifndef __entrant_include__
#define __entrant_include__

#include "object/chain.h"
#include "thread/coroutine.h"

class Entrant :
public Chain, public Coroutine
{
public:
	Entrant(const Entrant &copy) = delete; // prevent copying
	Entrant(void* tos) : Chain(), Coroutine(tos)
	{

	}
};

#endif
