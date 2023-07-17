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
#include "machine/plugbox.h"
#include "guard/gate.h"
#include "guard/guard.h"

#include "device/cgastr.h"
#include "machine/cpu.h"


/* FUNCTIONS */

extern "C" void guardian (unsigned int slot);

/* GUARDIAN: Low-level interrupt handling. We will extend this function at */
/*           a later point in time.                                        */

void guardian (unsigned int slot)
{
  // call the routine for the specific interrupt

  if (slot != plugbox.keyboard && slot != plugbox.timer && slot != plugbox.sb16) {
    kout << " guardian: " << dec << slot << endl;
  }

  Gate& gate = plugbox.report(slot);
  if (gate.prologue()) guard.relay(&gate);
}
