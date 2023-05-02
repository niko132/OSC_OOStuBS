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
    cpu.disable_int();
    kout.setPos(0, 3);
    // cpu.disable_int(); TODO: before or after setPos() needs to be discussed
    kout << "Test: " << dec << cnt;
    kout.flush();
    cpu.enable_int();
    cnt++;
  }
}
