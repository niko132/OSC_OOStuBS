/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  G A T E                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Class of objects that handle interrupts.                                  */
/*****************************************************************************/

#ifndef __Gate_include__
#define __Gate_include__

#include "object/chain.h"

class Gate : public Chain {
private:
    bool is_queued = false;
public: 
    virtual bool prologue() = 0;
    virtual void epilogue() {};

    void queued(bool q) {
        is_queued = q;
    };

    bool queued() {
        return is_queued;
    }
};

#endif
