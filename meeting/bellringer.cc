/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          B E L L R I N G E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The "bell ringer" (Bellringer) is activated periodically and checks if    */
/* any "bells" (Bell objects) need to ring. The bells are in a list that the */
/* bellringer manages.                                                       */
/*****************************************************************************/

#include "meeting/bellringer.h"

void Bellringer::check() {
    Bell* first = (Bell*)this->first();
    if (first == nullptr) return;

    first->tick();
    first = (Bell*)this->first();

    // use a loop to cover the case where 2 bells fire at the same time
    // (second one already has a counter of 0)
    while ((first = (Bell*)this->first()) != nullptr) {
        if (first->run_down()) {
            remove(first);
            first->ring();
        } else break;
    }
}

void Bellringer::job(Bell* bell, int ticks) {
    if (this->first() == nullptr) {
        bell->wait(ticks);
        insert_first(bell);
    } else {
        Bell* first = (Bell*)this->first();
        if (ticks < first->wait()) {
            bell->wait(ticks);
            first->wait(first->wait() - ticks);
            insert_first(bell);
        } else {
            Bell* before = first;
            Bell* after = nullptr;
            int beforeAbs = first->wait();
            while (before->next != nullptr) {
                after = (Bell*)before->next;
                int nextTicks = beforeAbs + after->wait();
                if (nextTicks > ticks) break;
                before = after;
                beforeAbs = nextTicks;
            }
            bell->wait(ticks - beforeAbs);
            if (after != nullptr) after->wait(after->wait() - bell->wait());
            insert_after(before, bell);
        }
    }
}

void Bellringer::cancel(Bell* bell) {
    if (bell->next != nullptr) {
        Bell* next = (Bell*)bell->next;
        next->wait(next->wait() + bell->wait());
    }

    remove(bell);
}

Bellringer bellringer;
