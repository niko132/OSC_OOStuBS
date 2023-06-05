/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          D I S P A T C H E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Dispatcher implementation.                                                */
/* The Dispatcher maintains the life pointer that points to the currently    */
/* active coroutine. go() initializes the life pointer and starts the first  */
/* coroutine, all further context switches are triggered by dispatch().      */
/* active() returns the life pointer.                                        */
/*****************************************************************************/

#include "thread/dispatch.h"


Dispatcher::Dispatcher(){
    lifePointer = nullptr; 
}

Coroutine* Dispatcher::active (){
    return lifePointer; 
}

void Dispatcher::dispatch (Coroutine& next){
    Coroutine* oldLife = lifePointer;
    lifePointer = &next;
    oldLife->resume(next);
}

void Dispatcher::go (Coroutine& first){
    lifePointer = &first; 
    first.go(); 
}

Dispatcher dispatcher; 


