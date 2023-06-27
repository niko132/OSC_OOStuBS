/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K E Y B O A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Keyboard driver.                                                          */
/*****************************************************************************/

#ifndef __Keyboard_include__
#define __Keyboard_include__

#include "machine/keyctrl.h"
#include "guard/gate.h"
#include "machine/key.h"
#include "machine/keyctrl.h"

#include "meeting/semaphore.h"

// a lock free ring buffer implementation to store keys
class BoundedKeyBuffer {
private:
  static const int SIZE = 10;
  Key buf[SIZE];
  int nextIn, nextOut;

public:
  BoundedKeyBuffer() : nextIn(0), nextOut(0) { };

  bool produce(Key key) {
    if ((nextIn + 1) % SIZE == nextOut) return false;
    buf[nextIn] = key;
    nextIn = (nextIn + 1) % SIZE;
    return true;
  };

  Key consume() {
    if (nextOut == nextIn) return Key();
    Key result = buf[nextOut];
    nextOut = (nextOut + 1) % SIZE;
    return result;
  };
};

class Keyboard : public Gate, public Keyboard_Controller
{
private:
  // the following vars are for repeat rate config
  const int speeds[7] = {
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

  BoundedKeyBuffer pressed_keys;
  int currentXPos, currentYPos;

  BoundedKeyBuffer getKeyBuf;
  Semaphore sem;

public:
	Keyboard();
	Keyboard(const Keyboard &copy) = delete; // prevent copying

	// PLUGIN: "Plugs in" the keyboard (driver). From now on, keypresses are handled.
	void plugin();
	bool prologue() override;
  void epilogue() override;

  Key getKey();

};

#endif
