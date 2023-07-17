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

	unsigned char* segmentStartAddress = nullptr;
	unsigned char* segmentEndAddress = nullptr;
	unsigned char* currentPixelAddress = nullptr;

	void writeRegisters(unsigned char* registers);
	unsigned char* getFrameBufferSegment();

public:
	VGA(const VGA &copy) = delete; // prevent copying
	VGA();

	bool supportsMode(unsigned int width, unsigned int height, unsigned int depth);
	bool setMode(unsigned int width, unsigned int height, unsigned int depth);

	inline unsigned char getColorIndex(unsigned char r, unsigned char g, unsigned char b) {
		unsigned char rVal = r / (255 / 3);
		unsigned char gVal = g / (255 / 7);
		unsigned char bVal = b / (255 / 7);
		
		return (bVal & 0x07) + ((gVal & 0x07) << 3) + ((rVal & 0x03) << 6);
	}
	
	void putPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
	void putPixel(unsigned int x, unsigned int y, unsigned char colorIndex);

	inline void putPixelFast(unsigned char r, unsigned char g, unsigned char b) {
		*(currentPixelAddress++) = getColorIndex(r, g, b);
	}

	inline void putPixelFast(unsigned char colorIndex) {
		*(currentPixelAddress++) = colorIndex;
	}

	inline void resetFastMode() {
		currentPixelAddress = segmentStartAddress;
	}

	void fillLine(unsigned int x, unsigned int y, unsigned int width, unsigned char colorIndex);
	void fillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char colorIndex);

	void setColor(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
	void loadDefaultPalette();

	void writeText(int x, int y, unsigned char* text);
	void writeChar(int x, int y, unsigned char c);
};

#endif
