/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                               B U Z Z E R                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* An "alarm clock" is a synchronization object that allows one or more      */
/* threads to put themselves to sleep for a specified time.                  */
/*****************************************************************************/

#include "meeting/buzzer.h"
#include "meeting/bellringer.h"
#include "thread/customer.h"
#include "syscall/guarded_organizer.h"
#include "device/watch.h"

Buzzer::Buzzer() {

}

Buzzer::~Buzzer() {
    bellringer.remove(this);
    ring(); // wake them up
}

void Buzzer::ring() {
    Customer* customer;
    while ((customer = (Customer*)this->dequeue()) != nullptr) {
        organizer.wakeup(*customer);
    }
}

void Buzzer::set(int ms) {
    this->timerMs = ms;
    int ticks = ms * 1000 / watch.interval(); // ms vs us
    bellringer.job(this, ticks);
}

void Buzzer::reset() {
    bellringer.remove(this);
    set(this->timerMs);
}

void Buzzer::sleep() {
    Customer* active = (Customer*)organizer.active();
    organizer.block(*active, *this);
}
