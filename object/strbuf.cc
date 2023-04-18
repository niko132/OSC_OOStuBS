/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       S T R I N G B U F F E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Stringbuffer class provides a buffer for collecting characters to be  */
/* printed to an output device, in our case the PC screen. The actual output */
/* occurs once the buffer is full, or when the user explicitly calls         */
/* flush().                                                                  */
/* As Stringbuffer is intended to be device independent, flush() is a        */
/* (pure) virtual method that must be defined by derived classes.            */
/*****************************************************************************/

#include "object/strbuf.h"

Stringbuffer::Stringbuffer() {
  pos = 0;
  for (int i = 0; i < 80; i++) {
    str[i] = 0;
  }
}

void Stringbuffer::put(char c) {
  str[pos++] = c;
  if (pos >= 80 || c == '\n') flush();
}
