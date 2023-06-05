/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                               L O C K E R                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Locker class implements a lock that can be used to protect critical   */
/* sections. However, the variable only indicates whether the critical       */
/* section is free. Potentially necessary waiting, and protection for        */
/* counting functionality, must be implemented elsewhere.                    */
/*****************************************************************************/

#ifndef __Locker_include__
#define __Locker_include__

#include "device/panic.h"
//for debug
#include "device/cgastr.h"


class Locker {
private:
	bool is_free;
public:
	Locker(const Locker &copy) = delete; // prevent copying
	Locker() : is_free(true) { };

	void enter() {
		if (!is_free) {kout.setPos(0,0); 
						kout<< "d"; 
						kout.flush(); 
		}
		if (!is_free) panic.prologue();
		is_free = false;
	};

	void retne() {
			if (is_free) {kout.setPos(0,0); 
						kout<< "f"; 
						kout.flush(); 
		}
		if (is_free) panic.prologue();
		is_free = true;
	};

	bool avail() {
		return is_free;
	};
};

#endif
