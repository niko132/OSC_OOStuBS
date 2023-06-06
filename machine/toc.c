/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 T O C                                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The toc struct is used to save the values of the non-volatile registers   */
/* in case of a context switch. toc_settle prepares the initial stack and    */
/* the toc struct for the first activation.                                  */
/*****************************************************************************/

#include "machine/toc.h"

// TOC_SETTLE: Prepares a coroutine context for its first activation.
void toc_settle(struct toc *regs, void *tos,
		void (*kickoff)(void *, void *, void *, void *, void *, void *,
				void *),
		void *object)
{
	// STACK LAYOUT
	//
	// LA	[					]
	// 		...
	// ^	[					]
	// |	[&kickoff			] <- rsp
	// |	[0					] <- bsp
	//		[object (1st arg)	] <- argp
	// HA	[					] <- tos

	// 1st: prepare all pointers
	void** p = (void**)tos;
	void** argp = --p;
	void** bp = --p;
	void** sp = --p;

	// 2nd: assign values to the pointers
	*argp = object;
	*bp = 0;
	*sp = (void*)kickoff;

	// 3rd: zero the registers and assign the corrent bp & sp
	regs->rbx = 0;
	regs->r12 = 0;
	regs->r13 = 0;
	regs->r14 = 0;
	regs->r15 = 0;
	regs->rbp = bp;
	regs->rsp = sp;
	for (int i = 0; i < 108; i++) {
		regs->fpu[i] = 0;
	}
}
