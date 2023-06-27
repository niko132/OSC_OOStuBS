/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          O R G A N I Z E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Organizer is a special scheduler that additionally allows processes   */
/* (Customer objects) to wait for events (Waitingroom objects).              */
/*****************************************************************************/

#include "thread/organizer.h"

void Organizer::block(Customer& customer, Waitingroom& waitingroom) {
    this->readyList.remove((Chain*)&customer); // remove every instance from the ready list
    waitingroom.enqueue((Chain*)&customer);
    customer.waiting_in(&waitingroom);
    exit(); // since we can not continue
}

void Organizer::wakeup(Customer& customer) {
    Waitingroom* w = customer.waiting_in();
    if (w != nullptr) w->remove(&customer);
    customer.waiting_in(nullptr);
    ready(customer);
    // no need to exit() here
}

void Organizer::kill(Customer& that) {
    if (that.waiting_in() != nullptr) {
        that.waiting_in()->remove(&that);
    }
    Scheduler::kill(that);
}
