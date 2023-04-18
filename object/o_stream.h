/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            O _ S T R E A M                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The O_Stream class defines the << operator for several pre-defined data   */
/* types and thereby realizes output functionality similar to C++'s iostream */
/* library. By default, this class supports printing characters, strings and */
/* integer numbers of various bit sizes. Another << operator allows to use   */
/* so-called 'manipulators'.                                                 */
/*                                                                           */
/* Besides class O_Stream this file also defines the manipulators hex, dec,  */
/* oct and bin for choosing the basis in number representations, and endl    */
/* for implementing an (implicitly flushing) line termination.               */
/*****************************************************************************/

#ifndef __o_stream_include__
#define __o_stream_include__

#include "object/strbuf.h"

class O_Stream
: public Stringbuffer
{
private:
 	unsigned long numberBase;

	char toPrintableDigit(unsigned long digit);

public:
	O_Stream(const O_Stream &copy) = delete; // prevent copying
	O_Stream();

	void setNumberBase(unsigned long base);

	O_Stream& operator<< (unsigned char c);
	O_Stream& operator<< (char c);

	O_Stream& operator<< (unsigned long number);
	O_Stream& operator<< (unsigned int number);
	O_Stream& operator<< (unsigned short number);

	O_Stream& operator<< (long number);
	O_Stream& operator<< (int number);
	O_Stream& operator<< (short number);

	O_Stream& operator<< (char* text);

	O_Stream& operator<< (void* pointer);

	O_Stream& operator<< (O_Stream& (*fkt) (O_Stream&));
};

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          M A N I P U L A T O R S                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The functions below all receive and return a reference to an O_Stream     */
/* object. Because class O_Stream defines an operator<< for functions with   */
/* this signature, they can be called in this operator's implementation and  */
/* even be embedded in a "stream" of outputs. A manipulator's task is to     */
/* influence subsequent output, e.g. by switching to a different number      */
/* system.                                                                   */
/*---------------------------------------------------------------------------*/

// ENDL: inserts a newline in the output and flushes the buffer
O_Stream& endl (O_Stream& os);

// BIN: selects the binary number system
O_Stream& bin (O_Stream& os);

// OCT: selects the octal number system
O_Stream& oct (O_Stream& os);

// DEC: selects the decimal number system
O_Stream& dec (O_Stream& os);

// HEX: selects the hexadecimal number system
O_Stream& hex (O_Stream& os);

#endif
