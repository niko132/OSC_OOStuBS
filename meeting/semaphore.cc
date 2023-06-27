/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           S E M A P H O R E                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The class Semaphore implements the synchronization concept of the         */
/* counting semaphore.                                                       */
/*****************************************************************************/

#include "meeting/semaphore.h"
#include "syscall/guarded_organizer.h"
#include "guard/secure.h"

#include "device/cgastr.h"

Semaphore::Semaphore(int c) : c(c) {
    
}

void Semaphore::p() {
    while(c <= 0) {
        Customer* customer = (Customer*)organizer.active();
        organizer.block(*customer, *this);
    }
    c--;
}

void Semaphore::v() {
    c++;
    Customer* customer;
    while ((customer = (Customer*)this->dequeue()) != nullptr) {
        organizer.wakeup(*customer);
    }
}
