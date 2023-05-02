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

#include "machine/pic.h"

PIC::PIC() : imr_1_port(0x21), imr_2_port(0xa1) {

}

void PIC::allow(int interrupt_device) {
  unsigned char offset;
  IO_Port port = cvt_device(interrupt_device, &offset);

  unsigned short imr = port.inb();
  imr = imr & ~(0x1 << offset);
  port.outb(imr);
}

void PIC::forbid(int interrupt_device) {
  unsigned char offset;
  IO_Port port = cvt_device(interrupt_device, &offset);

  unsigned char imr = port.inb();
  imr = imr | (0x1 << offset);
  port.outb(imr);
}

bool PIC::is_masked(int interrupt_device) {
  unsigned char offset;
  IO_Port port = cvt_device(interrupt_device, &offset);

  unsigned char imr = port.inb();
  return (imr >> offset) & 0x1;
}

bool PIC::get_and_forbid(int interrupt_device) {
  bool forbidden = is_masked(interrupt_device);
  forbid(interrupt_device);
  return forbidden;
}

void PIC::set_forbidden(int interrupt_device, bool forbidden) {
  if (forbidden)
    forbid(interrupt_device);
  else
    allow(interrupt_device);
}

const IO_Port& PIC::cvt_device(int interrupt_device, unsigned char* offset) {
  if (interrupt_device < 8) {
    *offset = interrupt_device;
    return imr_1_port;
  } else {
    *offset = interrupt_device - 8;
    return imr_2_port;
  }
}

PIC pic;
