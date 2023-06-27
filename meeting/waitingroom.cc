/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                        W A I T I N G R O O M                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implements a list of processes (Customer objects) that are all waiting    */
/* for a specific event.                                                     */
/*****************************************************************************/

#include "meeting/waitingroom.h"
#include "thread/customer.h"

#include "syscall/guarded_organizer.h"

Waitingroom::~Waitingroom() {
    Customer* customer = nullptr;
    while ((customer = (Customer*)this->dequeue()) != nullptr) {
        organizer.wakeup(*customer);
    }
}

void Waitingroom::remove(Customer* customer) {
    this->Queue::remove((Chain*)customer);

    // this method does not call wakeup
    // the removed customer should be woken up elswhere
}
