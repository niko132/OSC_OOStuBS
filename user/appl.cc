/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

/* INCLUDES */

#include "user/appl.h"
#include "device/cgastr.h"

#include "machine/cpu.h"
#include "guard/guard.h"
#include "guard/secure.h"
#include "thread/scheduler.h"
#include "thread/entrant.h"

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[5][STACK_SIZE];
char* mainStack = stacks[0];

Application::Application() : Entrant(&mainStack[STACK_SIZE]) {

}

void Application::action()
{
  kout.clear();

  // prepare some threads
  TestCoroutine cor1(1, &stacks[1][STACK_SIZE]);
  TestCoroutine cor2(2, &stacks[2][STACK_SIZE]);
  TestCoroutine cor3(3, &stacks[3][STACK_SIZE]);
  TestCoroutine cor4(4, &stacks[4][STACK_SIZE]);

  cor1.setKillPtr(&cor1); // kill self
  cor2.setKillPtr(&cor3); // kill other

  // exit the current execution and start the threads
  scheduler.exit();

  // this code won't be executed - it's here to demonstrate that scheduler.exit() works
  {
    Secure secure;

    kout.setPos(1, 5);
    kout << "This should not be displayed";
    kout.flush();
  }
}

TestCoroutine::TestCoroutine(int threadId, void* tos) : Entrant(tos), id(threadId)
{
  scheduler.ready(*this);
}

void TestCoroutine::action() {
  unsigned long cnt = 0;
  while (true) {
    if (this->killPtr && cnt >= 1000) {
      scheduler.kill(*killPtr);
    }

    {
      Secure secure;

      kout.setPos(1, id);
      kout << "Thread #" << dec << id << ": " << dec << cnt;
      kout.flush();
    }
    cnt++;

    scheduler.resume();
  }
}

void TestCoroutine::setKillPtr(Entrant* that) {
  this->killPtr = that;
}
