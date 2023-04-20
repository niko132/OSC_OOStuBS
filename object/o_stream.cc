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

#include "object/o_stream.h"

O_Stream::O_Stream() {
  numberBase = 10;
}

char O_Stream::toPrintableDigit(unsigned long digit) {
  if (digit > 9)
    return 'A' + digit - 10;
  return '0' + digit;
}

void O_Stream::setNumberBase(unsigned long base) {
  numberBase = base;
}

O_Stream& O_Stream::operator<< (unsigned char c) {
  put((char)c);
  return *this;
}

O_Stream& O_Stream::operator<< (char c) {
  put(c);
  return *this;
}

O_Stream& O_Stream::operator<< (unsigned long number) {
  unsigned long divider = 1;
  while (number / divider >= numberBase) {
    divider *= numberBase;
  }

  while (divider > 0) {
    unsigned long digit = (number / divider) % numberBase;
    put(toPrintableDigit(digit));
    divider /= numberBase;
  }
  return *this;
}

O_Stream& O_Stream::operator<< (unsigned int number) {
  return *this << ((unsigned long)number);
}

O_Stream& O_Stream::operator<< (unsigned short number) {
  return *this << ((unsigned long)number);
}

O_Stream& O_Stream::operator<< (long number) {
  if (number < 0) {
    if (numberBase == 10) {
      put('-');
      return *this << ((unsigned long)(number * -1));
    } else {
      return *this << ((unsigned long)number);
    }
  }
  return *this << ((unsigned long)number);
}

O_Stream& O_Stream::operator<< (int number) {
  return *this << ((long)number);
}

O_Stream& O_Stream::operator<< (short number) {
  return *this << ((long)number);
}

O_Stream& O_Stream::operator<< (char* text) {
  for (int i = 0; text[i] != 0; i++) {
    put(text[i]);
  }
  return *this;
}

O_Stream& O_Stream::operator<< (void* pointer) {
  unsigned long address = (unsigned long)pointer;
  return *this << '#' << address;
}

O_Stream& O_Stream::operator<< (O_Stream& (*fkt) (O_Stream&)) {
  return fkt(*this);
}

O_Stream& endl (O_Stream& os) {
  os.put('\n');
  os.flush();
  return os;
}

O_Stream& bin (O_Stream& os) {
  os.setNumberBase(2);
  return os;
}

O_Stream& oct (O_Stream& os) {
  os.setNumberBase(8);
  return os;
}
O_Stream& dec (O_Stream& os) {
  os.setNumberBase(10);
  return os;
}
O_Stream& hex (O_Stream& os) {
  os.setNumberBase(16);
  return os;
}
