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
    /*
    cpu.enable_int(); // TODO: find best spot to enable interrupts

    Chain* elem = nullptr;
    while ((elem = queue.dequeue()) != nullptr) {
        Gate* gate = (Gate*) elem;
        gate->epilogue();
        gate->queued(false); // TODO: check ordering
    }

    retne();
    */


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
    // TODO: discuss and choose better solution
    /*
    if (!item->queued())
        queue.enqueue(item);

    if (avail()) {
        enter(); // enter critical section
        leave(); // just to run every queued epilogue
    }
    */

    if (avail()) {
        item->epilogue();
    } else if (!item->queued()) {
        queue.enqueue(item);
        item->queued(true);
    }
}

Guard guard;
