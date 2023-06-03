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
	// 1. Step:
	// zero registers (toc)
	regs->rbx = 0;
	regs->r12 = 0;
	regs->r13 = 0;
	regs->r14 = 0;
	regs->r15 = 0;
	regs->rbp = tos - 2 * sizeof(void*); // TODO: find out why exactly
	// it's not important which value is assigned to the base pointer
	// even though the position of the params should be determined by it

	// also our stack layout is slightly different than the ones online
	// online is one more entry for the prev base address
	// this should theoretically also mess up the programm

	// but idk - it works
	regs->rsp = tos - 3 * sizeof(void*);
	for (int i = 0; i < 108; i++) {
		regs->fpu[i] = 0;
	}

	// 2. Step:
	// prepare the stack
	void** p = (void**) tos;
	p--;
	*p = object;
	p--;
	*p = 0;
	p--;
	*p = (void*)kickoff;
}
