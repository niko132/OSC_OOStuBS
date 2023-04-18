#ifndef __screen_include__
#define __screen_include__

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

#include "machine/io_port.h"

enum class CGA_Color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARK_GRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

class CGA_Screen {
private:
  IO_Port indexPort;
  IO_Port dataPort;

public:
	CGA_Screen(const CGA_Screen &copy) = delete; // prevent copying
	CGA_Screen();

	void show(int x, int y, char c, unsigned char attrib = 0x0f);
	void setPos(int x, int y);
	void getPos(int &x, int &y);
	void print(char *text, int length, unsigned char attrib = 0x0f);
	void scrollDown(int blankLines = 1);
	void clear();

	static unsigned char getAttrib(CGA_Color fg, CGA_Color bg, bool blinking);
};

#endif
