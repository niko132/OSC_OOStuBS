#include "device/cgastr.h"
#include "machine/keyctrl.h"

int main()
{
  int speeds[] = {
    0x14,
    0x10,
    0x0c,
    0x08,
    0x04,
    0x02,
    0x00
  };

  int currentDelay = 0;
  int currentSpeedIndex = 0;
  int currentSpeed = speeds[currentSpeedIndex];

  Keyboard_Controller kc;
  kc.set_repeat_rate(currentSpeed, currentDelay); // set initial repeat rate

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

  while (true) {
    Key key = kc.key_hit();
    if (!key.valid()) continue;

    if (key.scancode() == 75) {
      currentDelay--;
      if (currentDelay < 0) currentDelay = 0;
      kc.set_repeat_rate(currentSpeed, currentDelay);
      continue;
    } else if (key.scancode() == 77) {
      currentDelay++;
      if (currentDelay > 3) currentDelay = 3;
      kc.set_repeat_rate(currentSpeed, currentDelay);
      continue;
    } else if (key.scancode() == 80) {
      currentSpeedIndex--;
      if (currentSpeedIndex < 0) currentSpeedIndex = 0;
      currentSpeed = speeds[currentSpeedIndex];
      kc.set_repeat_rate(currentSpeed, currentDelay);
      continue;
    } else if (key.scancode() == 72) {
      currentSpeedIndex++;
      if (currentSpeedIndex > 6) currentSpeedIndex = 6;
      currentSpeed = speeds[currentSpeedIndex];
      kc.set_repeat_rate(currentSpeed, currentDelay);
      continue;
    } else if (key.scancode() == 1) {
      kc.reboot();
      continue;
    }

    kout << key.ascii();
    kout.flush();
  }

	return 0;
}
