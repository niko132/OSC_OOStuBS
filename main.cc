#include "device/cgastr.h"
#include "machine/keyctrl.h"


#include "device/keyboard.h"
#include "machine/pic.h"
#include "machine/cpu.h"
#include "thread/dispatch.h"
#include "user/appl.h"


int main()
{
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
  pic.allow(PIC::keyboard);

  Application application;
  dispatcher.go(application); 

	return 0;
}
