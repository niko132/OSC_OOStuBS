/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                G U A R D                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Using this class, we can synchronize activities with a critical section   */
/* with interrupt handlers that also access this critical section. This      */
/* synchronization takes place along the prologue/epilogue model.            */
/*****************************************************************************/

#include "guard/guard.h"
#include "machine/cpu.h"

void Guard::leave() {
    Gate* g;
    while (true) {
        cpu.disable_int();
        g = (Gate*)queue.dequeue();
        if (g) {
            cpu.enable_int();
            g->queued(false);
            g->epilogue();
        } else {
            break;
        }
    }
    retne();
    cpu.enable_int();
}

void Guard::relay(Gate* item) {
    // at this point we assume that interrupts got disabled by the interrupt handler (assembler)
    // therefore we can guarantee synchronized access to the queue
    if (!item->queued())
        queue.enqueue(item);

    if (avail()) {
        enter(); // enter the critical section so that other interrupts don't call their epilogues
        cpu.enable_int(); // not really needed because leave() handles interrupt enable/disable but maybe good for realtime optimizations
        // interrupts can trigger here and enqueue their epilogues so that they are handled faster
        leave();
    }

    // we queued our epilogue
    // now either   1. the CS was available and the epilogues got called already
    // or           2. the system will call leave() soon and handle all queued epilogues
}

Guard guard;