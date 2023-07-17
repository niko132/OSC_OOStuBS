/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  P I C                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Programmable Interrupt Controller.                                        */
/* By using the PIC, hardware interrupts can be enabled or suppressed        */
/* individually. This way we can control whether interrupts of a specific    */
/* device are forwarded to the CPU at all. Even then, the interrupt handler  */
/* gets only activated if the CPU is ready to react to interrupts. This can  */
/* be controlled by using class CPU.                                         */
/*****************************************************************************/

#ifndef __pic_include__
#define __pic_include__

#include "machine/io_port.h"

class PIC {
private:
		// the two used ports of the pic
		const IO_Port imr_1_port; // interrupt mask register 1
		const IO_Port imr_2_port; // interrupt mask register 2

		const IO_Port& cvt_device(int interrupt_device, unsigned char* offset);

public:
	// status-register bits
	enum { timer = 0x00, keyboard = 0x01, sb16 = 0x05 };

	PIC(const PIC &copy) = delete; // prevent copying
	PIC();

	void allow(int interrupt_device);
	void forbid(int interrupt_device);
	bool is_masked(int interrupt_device);

	bool get_and_forbid(int interrupt_device);
	void set_forbidden(int interrupt_device, bool forbidden);
};

extern PIC pic;

#endif
