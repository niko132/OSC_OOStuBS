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

#include "graphics/vga.h"
#include "object/memory.h"

VGA::VGA() :
    miscPort(0x3c2),
    crtcIndexPort(0x3d4),
    crtcDataPort(0x3d5),
    sequencerIndexPort(0x3c4),
    sequencerDataPort(0x3c5),
    graphicsControllerIndexPort(0x3ce),
    graphicsControllerDataPort(0x3cf),
    attributeControllerIndexPort(0x3c0),
    attributeControllerReadPort(0x3c1),
    attributeControllerWritePort(0x3c0),
    attributeControllerResetPort(0x3da),

    pelIndexPort(0x3c8),
    pelDataPort(0x3c9)
{

}

void VGA::writeRegisters(unsigned char* registers) {
    // misc
    miscPort.outb(*(registers++));

    // sequencer
    for (int i = 0; i < 5; i++) {
        sequencerIndexPort.outb(i);
        sequencerDataPort.outb(*(registers++));
    }

    // crtc
    crtcIndexPort.outb(0x03);
    crtcDataPort.outb(crtcDataPort.inb() | 0x80);
    crtcIndexPort.outb(0x11);
    crtcDataPort.outb(crtcDataPort.inb() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for (int i = 0; i < 25; i++) {
        crtcIndexPort.outb(i);
        crtcDataPort.outb(*(registers++));
    }

    // graphics controller
    for (int i = 0; i < 9; i++) {
        graphicsControllerIndexPort.outb(i);
        graphicsControllerDataPort.outb(*(registers++));
    }

    // attribute controller
    for (int i = 0; i < 21; i++) {
        attributeControllerResetPort.inb();
        attributeControllerIndexPort.outb(i);
        attributeControllerWritePort.outb(*(registers++));
    }

    attributeControllerResetPort.inb();
    attributeControllerIndexPort.outb(0x20);
}

bool VGA::supportsMode(unsigned int width, unsigned int height, unsigned int depth) {
    return (width == 320 && height == 200 && depth == 8) || (width == 80 && height == 25 && depth == 1);
}

bool VGA::setMode(unsigned int width, unsigned int height, unsigned int depth) {
    if (!supportsMode(width, height, depth)) return false;

    unsigned char g_320x200x256[] =
    {
        // misc
        0x63,
        // seq
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        // crtc
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
        0xFF,
        // gc
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        // ac
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00, 0x00
    };

    unsigned char g_80x25_text[] =
    {
    /* MISC */
        0x67,
    /* SEQ */
        0x03, 0x00, 0x03, 0x00, 0x02,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
        0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
        0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x0C, 0x00, 0x0F, 0x08, 0x00
    };

    unsigned char* regs = nullptr;
    if (width == 320 && height == 200 && depth == 8) {
        regs = g_320x200x256;
    } else if (width == 80 && height == 25 && depth == 1) {
        regs = g_80x25_text;
    }

    writeRegisters(regs);

    segmentStartAddress = getFrameBufferSegment();
    segmentEndAddress = segmentStartAddress + 320 * 200;
    currentPixelAddress = segmentStartAddress;

    return true;
}

unsigned char* VGA::getFrameBufferSegment() {
    graphicsControllerIndexPort.outb(0x06);
    unsigned char segmentNumber = graphicsControllerDataPort.inb() & (3 << 2);
    switch (segmentNumber) {
        default:
        case 0 << 2: return (unsigned char*)0x00000;
        case 1 << 2: return (unsigned char*)0xA0000;
        case 2 << 2: return (unsigned char*)0xB0000;
        case 3 << 2: return (unsigned char*)0xB8000;
    }
}

void VGA::putPixel(unsigned int x, unsigned int y, unsigned char colorIndex) {
    if (320 <= x || 200 <= y) return;

    unsigned char* pixelAddress = segmentStartAddress + 320 * y + x;
    *pixelAddress = colorIndex;
}

void VGA::putPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
    putPixel(x, y, getColorIndex(r, g, b));
}

void VGA::fillLine(unsigned int x, unsigned int y, unsigned int width, unsigned char colorIndex) {
    if (x >= 320) x = 319;
    if (y >= 200) y = 199;
    if (x + width >= 320) width = 320 - x;

    unsigned char* pixelAddress = segmentStartAddress + 320 * y + x;
    memset(pixelAddress, colorIndex, width);
}

void VGA::fillRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char colorIndex) {
    if (x >= 320) x = 319;
    if (y >= 200) y = 199;
    if (x + width >= 320) width = 320 - x;
    if (y + height >= 200) height = 200 - y;

    for (int i = 0; i < height; i++) {
        fillLine(x, y + i, width, colorIndex);
    }
}

void VGA::setColor(unsigned char index, unsigned char r, unsigned char g, unsigned char b) {
    pelIndexPort.outb(index);
    pelDataPort.outb(r);
    pelDataPort.outb(g);
    pelDataPort.outb(b);
}

void VGA::loadDefaultPalette() {
    for (int i = 0; i < 256; i++) {
        unsigned char r = ((i >> 6) & 0x03);
        unsigned char g = (i >> 3) & 0x07;
        unsigned char b = i & 0x07;

        unsigned char rVal = 63 * r / 3;
        unsigned char gVal = 63 * g / 7;
        unsigned char bVal = 63 * b / 7;

        setColor(i, rVal, gVal, bVal);
    }
}

void VGA::writeText(int x, int y, unsigned char* text) {
    while(*text != 0) {
        writeChar(x, y, *text);
        x += 8;
        text++;
    }
}

unsigned char ALPH[24][8] = {
    {
        0b00001000,
        0b00011000,
        0b00101000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00111110,
        0b00000000,
    },
    {
        0b00011100,
        0b00100010,
        0b00000010,
        0b00001100,
        0b00010000,
        0b00100000,
        0b00111110,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111110,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00011110,
        0b00100000,
        0b00011100,
        0b00000010,
        0b00111100,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00011100,
        0b00100010,
        0b00100000,
        0b00100010,
        0b00011100,
        0b00000000,
    },
    {
        0b00100000,
        0b00100000,
        0b00101100,
        0b00110010,
        0b00100010,
        0b00100010,
        0b00100010,
        0b00000000,
    },
    {
        0b00001000,
        0b00000000,
        0b00000000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00101100,
        0b00110010,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00110100,
        0b00101010,
        0b00101010,
        0b00100010,
        0b00100010,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00101100,
        0b00110010,
        0b00100010,
        0b00111100,
        0b00100000,
        0b00100000,
    },
    {
        0b00000000,
        0b00000000,
        0b00011100,
        0b00100010,
        0b00111110,
        0b00100000,
        0b00011110,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00011110,
        0b00100010,
        0b00100010,
        0b00011110,
        0b00000010,
        0b00111100,
    },
    {
        0b00100000,
        0b00100000,
        0b00100100,
        0b00101000,
        0b00110000,
        0b00101000,
        0b00100100,
        0b00000000,
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {
        0b00000000,
        0b00100010,
        0b00100010,
        0b00100010,
        0b00100010,
        0b00010100,
        0b00010100,
        0b00001000,
    },
    {
        0b00000000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00111110,
    },
    {
        0b00000000,
        0b00011100,
        0b00100010,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100010,
        0b00011100,
    }
};

void VGA::writeChar(int startX, int startY, unsigned char c) {
    unsigned char* alph_char = nullptr;
    switch (c)
    {
        case '1':
            alph_char = ALPH[0];
            break;
        case '2':
            alph_char = ALPH[1];
            break;
        case '-':
            alph_char = ALPH[2];
            break;
        case 's':
            alph_char = ALPH[3];
            break;
        case 'c':
            alph_char = ALPH[4];
            break;
        case 'h':
            alph_char = ALPH[5];
            break;
        case 'i':
            alph_char = ALPH[6];
            break;
        case 'r':
            alph_char = ALPH[7];
            break;
        case 'm':
            alph_char = ALPH[8];
            break;
        case '.':
            alph_char = ALPH[9];
            break;
        case 'p':
            alph_char = ALPH[10];
            break;
        case 'e':
            alph_char = ALPH[11];
            break;
        case 'g':
            alph_char = ALPH[12];
            break;
        case 'k':
            alph_char = ALPH[13];
            break;
        case ' ':
            alph_char = ALPH[14];
            break;
        case 'V':
            alph_char = ALPH[15];
            break;
        case 'L':
            alph_char = ALPH[16];
            break;
        case 'C':
            alph_char = ALPH[17];
            break;
        
    }

        for (int y = 0; y < 8; y++) {
            unsigned char row = alph_char[y];
            for (int x = 0; x < 8; x++) {
                unsigned char colorIndex = (row >> (7 - x) & 0x01) ? 255 : 0;
                putPixel(startX + x, startY + y, colorIndex);
            }
        }
}
