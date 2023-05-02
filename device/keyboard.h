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

class Keyboard : public Gate, public Keyboard_Controller
{
private:
  Keyboard_Controller kc;

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

public:
	Keyboard();
	Keyboard(const Keyboard &copy) = delete; // prevent copying
/* Add your code here */

/* Add your code here */

	// PLUGIN: "Plugs in" the keyboard (driver). From now on, keypresses are handled.
	void plugin();

	void trigger () override;

};

extern Keyboard keyboard;

#endif
