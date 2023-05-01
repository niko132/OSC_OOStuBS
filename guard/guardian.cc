/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              G U A R D I A N                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The system's central interrupt handling routine.                          */
/* The parameter specifies the number of the interrupt that occurred.        */
/*****************************************************************************/

/* INCLUDES */
#include "device/cgastr.h"
#include "machine/plugbox.h"
#include "guard/gate.h"


/* FUNCTIONS */

extern "C" void guardian (unsigned int slot);

/* GUARDIAN: Low-level interrupt handling. We will extend this function at */
/*           a later point in time.                                        */

void guardian (unsigned int slot)
{
  //kout << "GUARDIAN: " << dec << slot << endl; // debug output

  // call the routine for the specific interrupt
  plugbox.report(slot).trigger(); 

}
