#include "device/cgastr.h"
#include "machine/keyctrl.h"


#include "device/keyboard.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "syscall/guarded_organizer.h"
#include "user/appl.h"

#include "guard/secure.h"


int main()
{
  // main() is a kernel level function so we run it on the epilogue level using a Secure object
  Secure secure;

  kout.clear();
  kout.setPos(0, 0);
  kout << "Booted - running tests:" << endl << endl;

  long test1 = 1234567L;
  kout << "BIN: " << bin << test1 << endl;
  kout << "OCT: " << oct << test1 << endl;
  kout << "DEC: " << dec << test1 << endl;
  kout << "HEX: " << hex << test1 << endl;
  kout << "ADDR: " << &test1 << endl << endl;

  long test2 = -1L;
  kout << "BIN: " << bin << test2 << endl;
  kout << "OCT: " << oct << test2 << endl;
  kout << "DEC: " << dec << test2 << endl;
  kout << "HEX: " << hex << test2 << endl;
  kout << "ADDR: " << &test2 << endl << endl;

  kout.print("COLORFUL\n", 9, (unsigned char)0xE2);

  kout << endl << "Press any key..." << endl;
  kout << "Use the arrow keys to control repeat rate" << endl << endl;


  cpu.enable_int();
  keyboard.plugin();

  Application application;

  // run the non guarded scheduler to not enter the critical section again
  organizer.Organizer::ready(application);
  organizer.schedule();

	return 0;
}
