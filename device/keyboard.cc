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

bool Keyboard::prologue() {
    bool keyFetched = false;
    while (true) {
        Key key = key_hit();
        if (!key.valid()) break;
        pressed_keys.produce(key);
        keyFetched = true;
    }

    return keyFetched;
}

void Keyboard::epilogue() {
    while (true) {
        Key key = pressed_keys.consume();
        if (!key.valid()) break;

        int x, y;
        kout.getPos(x, y);
        kout.setPos(7, 7);

        if (key.scancode() == 83) {
            if (key.ctrl_left() && key.alt_left()) // CTRL + ALT + DEL
                reboot();
        } else if (key.scancode() == 75) {
            currentDelay--;
            if (currentDelay < 0) currentDelay = 0;
            set_repeat_rate(currentSpeed, currentDelay);
        } else if (key.scancode() == 77) {
            currentDelay++;
            if (currentDelay > 3) currentDelay = 3;
            set_repeat_rate(currentSpeed, currentDelay);
        } else if (key.scancode() == 80) {
            currentSpeedIndex--;
            if (currentSpeedIndex < 0) currentSpeedIndex = 0;
            currentSpeed = speeds[currentSpeedIndex];
            set_repeat_rate(currentSpeed, currentDelay);
        } else if (key.scancode() == 72) {
            currentSpeedIndex++;
            if (currentSpeedIndex > 6) currentSpeedIndex = 6;
            currentSpeed = speeds[currentSpeedIndex];
            set_repeat_rate(currentSpeed, currentDelay);
        } else {
            kout << "Valid key: " << key.ascii();
            kout.flush();
        }

        kout.setPos(x, y);
    }
}

Keyboard keyboard;
