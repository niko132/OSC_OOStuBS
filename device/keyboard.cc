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
#include "machine/pic.h"
#include "device/cgastr.h"

Keyboard::Keyboard() : currentXPos(7), currentYPos(7), sem(0) {

}

void Keyboard::plugin(){
    plugbox.assign(plugbox.keyboard, *this);
    pic.allow(PIC::keyboard);
}

bool Keyboard::prologue() {
    bool keyFetched = false;
    while (true) {
        Key key = key_hit();
        if (!key.valid()) break;
        if (key.scancode() == 83 && key.ctrl_left() && key.alt_left()) // CTRL + ALT + DEL
                reboot();
        pressed_keys.produce(key);
        keyFetched = true;
    }

    return keyFetched;
}

void Keyboard::epilogue() {
    while (true) {
        Key key = pressed_keys.consume();
        if (!key.valid()) break;
        if (getKeyBuf.produce(key)) sem.signal();
    }
}

Key Keyboard::getKey() {
    sem.wait();
    Key key = getKeyBuf.consume();
    return key;
}
