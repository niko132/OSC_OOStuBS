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
    return;


//     int currentDelay = 0;
//   int currentSpeed = 0;
//   int currentSpeedIndex = 0;
  
//     Key key = key_hit();
//     if (!key.valid()) return; 
    //     if (key.scancode() == 75) {
    //   currentDelay--;
    //   if (currentDelay < 0) currentDelay = 0;
    //   set_repeat_rate(currentSpeed, currentDelay);
    //   return;
    // } else if (key.scancode() == 77) {
    //   currentDelay++;
    //   if (currentDelay > 3) currentDelay = 3;
    //   set_repeat_rate(currentSpeed, currentDelay);
    //   return;
    // } else if (key.scancode() == 80) {
    //   currentSpeedIndex--;
    //   if (currentSpeedIndex < 0) currentSpeedIndex = 0;
    //   //currentSpeed = speeds[currentSpeedIndex];
    //   set_repeat_rate(currentSpeed, currentDelay);
    //   return;
    // } else if (key.scancode() == 72) {
    //   currentSpeedIndex++;
    //   if (currentSpeedIndex > 6) currentSpeedIndex = 6;
    //   //currentSpeed = speeds[currentSpeedIndex];
    //   set_repeat_rate(currentSpeed, currentDelay);
    //   return;
    // } else if (key.scancode() == 1) {
    //   //reboot();
      
    //  }

    // kout << key.ascii();
    // kout.flush();
}

Keyboard keyboard; 