/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             C G A _ S C R E E N                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* This class allows accessing the PC's screen.  Accesses work directly on   */
/* the hardware level, i.e. via the video memory and the graphic adapter's   */
/* I/O ports.                                                                */
/*****************************************************************************/

#include "machine/cgascr.h"
#include "machine/io_port.h"
#include "object/memory.h"

CGA_Screen::CGA_Screen() : indexPort(0x3d4), dataPort(0x3d5)
{

}

void CGA_Screen::show(int x, int y, char c, unsigned char attrib) {
  char *CGA_START = (char *)0xb8000;
  char *pos = CGA_START + 2 * (x + y * 80);
  *pos = c;
  *(pos + 1) = (char)attrib;
}

void CGA_Screen::setPos(int x, int y) {
  unsigned short index = x + y * 80;
  char low = index & 0xff;
  char high = (index >> 8) & 0xff;

  indexPort.outb(14); // set high byte index
  dataPort.outb(high);

  indexPort.outb(15); // set low byte index
  dataPort.outb(low);
}

void CGA_Screen::getPos(int &x, int &y) {
  indexPort.outb(14); // set high byte index
  unsigned char high = (unsigned char)dataPort.inb();

  indexPort.outb(15); // set low byte index
  unsigned char low = (unsigned char)dataPort.inb();

  unsigned short index = high << 8 | low;
  x = index % 80;
  y = index / 80;
}

void CGA_Screen::print(char *text, int length, unsigned char attrib) {
  int x, y;
  getPos(x, y);

  for (int i = 0; i < length; i++) {
    if (x >= 80 || text[i] == '\n') {
      x = 0;
      y++;
    }
    if (y >= 25) {
      scrollDown();
      y = 24;
    }
    if (text[i] == '\n') continue;

    show(x, y, text[i], attrib);
    x++;
  }

  setPos(x, y);
}

void CGA_Screen::scrollDown(int blankLines) {
  if (blankLines > 25) blankLines = 25;
  unsigned int copyLines = 25 - blankLines;
  char* CGA_START = (char *)0xb8000;

  char* srcPtr = CGA_START + blankLines * 80 * 2;
  memcpy(CGA_START, srcPtr, copyLines * 80 * 2); // copy all bytes from copyLines rows * 80 chars per row * 2 byte per char

  char* blankPtr = CGA_START + copyLines * 80 * 2;
  unsigned short blankVal = (0x0f << 8) | ' ';
  memset(blankPtr, blankVal, blankLines * 80); // set all chars to the specified value -> short uses 2 byte per value
}

void CGA_Screen::clear() {
  scrollDown(25); // scroll all 25 lines
}

unsigned char CGA_Screen::getAttrib(CGA_Color fg, CGA_Color bg, bool blinking) {
  char fgChar = static_cast<int>(fg);
  char bgChar = static_cast<int>(bg);
  char blinkingChar = blinking ? 1 : 0;

  return (blinkingChar & 0x01) << 7 | (bgChar & 0x0f) << 4 | (fgChar & 0x0f);
}
