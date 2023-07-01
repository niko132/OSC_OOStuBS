/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                   V G A                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The VGA driver                 .                                          */
/* Currently allows enabling the graphics mode.                              */
/*****************************************************************************/

#ifndef __VGA_include__
#define __VGA_include__

#include "machine/io_port.h"

class VGA {
private:
	const IO_Port miscPort;
	const IO_Port crtcIndexPort;
	const IO_Port crtcDataPort;
	const IO_Port sequencerIndexPort;
	const IO_Port sequencerDataPort;
	const IO_Port graphicsControllerIndexPort;
	const IO_Port graphicsControllerDataPort;
	const IO_Port attributeControllerIndexPort;
	const IO_Port attributeControllerReadPort;
	const IO_Port attributeControllerWritePort;
	const IO_Port attributeControllerResetPort;

	const IO_Port pelIndexPort;
	const IO_Port pelDataPort;

	unsigned char* frameBufferSegment = nullptr;

	void writeRegisters(unsigned char* registers);
	unsigned char* getFrameBufferSegment();

public:
	VGA(const VGA &copy) = delete; // prevent copying
	VGA();

	bool supportsMode(unsigned int width, unsigned int height, unsigned int depth);
	bool setMode(unsigned int width, unsigned int height, unsigned int depth);

	unsigned char getColorIndex(unsigned char r, unsigned char g, unsigned char b);
	
	void putPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
	void putPixel(unsigned int x, unsigned int y, unsigned char colorIndex);

	void fillLine(unsigned int x, unsigned int y, unsigned int width, unsigned char colorIndex);
	void fillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char colorIndex);

	void setColor(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
	void loadDefaultPalette();
};

#endif
