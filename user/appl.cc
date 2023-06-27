/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

// GRUPPE 13!!!

/* INCLUDES */

#include "user/appl.h"
#include "device/cgastr.h"

#include "machine/cpu.h"
#include "guard/guard.h"
#include "guard/secure.h"
#include "syscall/guarded_organizer.h"
#include "thread/entrant.h"
#include "device/watch.h"

#include "syscall/guarded_semaphore.h"

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[5][STACK_SIZE];
char* mainStack = stacks[0];

Guarded_Semaphore screen_sem(1);

Application::Application() : Thread(&mainStack[STACK_SIZE]) {

}

void Application::action()
{
  kout.clear();

  // prepare some threads
  TestThread t1(1, &stacks[1][STACK_SIZE]);
  TestThread t2(2, &stacks[2][STACK_SIZE]);
  TestThread t3(3, &stacks[3][STACK_SIZE]);
  TestThread t4(4, &stacks[4][STACK_SIZE]);

  t1.setKillPtr(&t1); // kill self
  t2.setKillPtr(&t3); // kill other

  // CAUTION: 20us is not enough time for Virtual Box
  Watch watch(20); // 20us timer
  watch.windup();

  // exit the current execution and start the threads
  organizer.exit();

  // this code won't be executed - it's here to demonstrate that scheduler.exit() works
  {
    Secure secure;

    kout.setPos(1, 5);
    kout << "This should not be displayed";
    kout.flush();
  }
}

TestThread::TestThread(int threadId, void* tos) : Thread(tos), id(threadId)
{
  organizer.ready(*this);
}

void TestThread::action() {
  unsigned long cnt = 0;
  while (true) {
    if (this->killPtr && cnt >= 1000) {
      organizer.kill(*killPtr);
    }

    // needed for VirtualBox cause otherwise the mutexed section is much longer than the
    // non-mutexed section and the other threads would starve
    /*
    for (int i = 0; i < 1000000; i++) {
      i = i + 1 - 2 + 1;
    }
    */

    screen_sem.wait();

    kout.setPos(1, id);
    kout << "Thread #" << dec << id << ": " << dec << cnt;
    kout.flush();

    screen_sem.signal();

    // organizer.resume(); // TODO: can be used to allow fair scheduling (kinda no preemption)
    
    cnt++;
  }
}

void TestThread::setKillPtr(Thread* that) {
  this->killPtr = that;
}
