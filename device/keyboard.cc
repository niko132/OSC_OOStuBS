/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K E Y B O A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Keyboard driver.                                                          */
/*****************************************************************************/

#include "device/keyboard.h"
#include "machine/plugbox.h"
#include "device/cgastr.h"

Keyboard::Keyboard(){

}

void Keyboard::plugin(){
    plugbox.assign(plugbox.keyboard, *this);
}

void Keyboard::trigger(){
    kout << "Keyboard Interrupt!" << endl;

    int max_iter = 10; // avoid infinite loop
    Key key;
    // use a loop to fetch every key press (in case of missed interrupts)
    while (max_iter-- > 0) {
      key = kc.key_hit();
      if (!key.valid()) break;

      if (key.scancode() == 83) {
        if (key.ctrl_left() && key.alt_left()) // CTRL + ALT + DEL
          kc.reboot();
      } else if (key.scancode() == 75) {
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
      }

      kout << "Valid key: " << key.ascii() << endl;
    }

    return;
}

Keyboard keyboard;
