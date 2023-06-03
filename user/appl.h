/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

#ifndef __application_include__
#define __application_include__

#include "thread/coroutine.h"

class Application : public Coroutine

{

public:
	Application (const Application &copy) = delete; // prevent copying
  	Application();

	void action ();
};

class TestCoroutine : public Coroutine
{
public:
	int val;
	Coroutine& next;

	TestCoroutine(int startVal, void* tos, Coroutine& next);

	void action();
};

#endif
