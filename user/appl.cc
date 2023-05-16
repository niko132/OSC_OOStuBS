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
/* Add your code here */

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

Application::Application() {

}

void Application::action()
{
  kout.clear();

  unsigned long cnt = 0;
  while (true) {
    guard.enter();
    kout.setPos(0, 3);
    kout << "Test: " << dec << cnt;
    kout.flush();
    guard.leave();
    cnt++;
  }
}
