/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Scheduler implementation.                                                 */
/*****************************************************************************/

#include "thread/scheduler.h"
#include "device/cgastr.h"
#include "machine/cpu.h"

Scheduler::Scheduler() {

}

void Scheduler::ready(Entrant& that) {
    readyList.enqueue(&that);
}

void Scheduler::schedule() {
    Entrant* entrant = (Entrant*)readyList.dequeue();
    if (entrant == nullptr) nothingHandler();

    Dispatcher::go(*entrant);
}

void Scheduler::exit() {
    Entrant* entrant = (Entrant*)readyList.dequeue();
    if (entrant == nullptr) nothingHandler();
    Dispatcher::dispatch(*entrant);
}

void Scheduler::kill(Entrant& that) {
    readyList.remove(&that);

    // if the currently active thread kills itself
    // just schedule the next one in the list
    if (active() == &that) exit();
}

void Scheduler::resume() {
    Entrant* activeEntrant = (Entrant*)active();
    ready(*activeEntrant);
    exit();
}

void Scheduler::nothingHandler() {
    kout.clear();
    kout.setPos(0, 1);
    kout << "Nothing to schedule..." << endl;
    kout << "Stopping the execution!" << endl;
    cpu.halt();
}
