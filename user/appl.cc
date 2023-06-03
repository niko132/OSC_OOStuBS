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
/* Add your code here */

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[5][STACK_SIZE];
char* mainStack = stacks[0];

Application::Application() : Coroutine(&mainStack[STACK_SIZE]) {

}

void Application::action()
{
  kout.clear();

  TestCoroutine cor4(6, &stacks[4][STACK_SIZE], *this);
  TestCoroutine cor3(5, &stacks[3][STACK_SIZE], cor4);
  TestCoroutine cor2(4, &stacks[2][STACK_SIZE], cor3);
  TestCoroutine cor1(3, &stacks[1][STACK_SIZE], cor2);

  resume(cor1);

  unsigned long cnt = 0;
  while (true) {
    {
      Secure secure;

      kout.setPos(0, 2);
      kout << "Test: " << dec << cnt;
      kout.flush();
    }
    cnt++;
  }
}

TestCoroutine::TestCoroutine(int startVal, void* tos, Coroutine& next) : Coroutine(tos), val(startVal), next(next)
{

}

void TestCoroutine::action() {
  {
    Secure secure;

    kout.setPos(1, val);
    for (int i = 0; i < 10; i++) {
      kout << " " << dec << (val + i);
    }
    kout.flush();
  }
  resume(next);
}
