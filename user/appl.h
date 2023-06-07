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

#include "thread/entrant.h"

class Application : public Entrant

{

public:
	Application (const Application &copy) = delete; // prevent copying
  	Application();

	void action ();
};

class TestCoroutine : public Entrant
{
public:
	int id;
	Entrant* killPtr = nullptr;

	TestCoroutine(int threadId, void* tos);

	void action();
	void setKillPtr(Entrant* that);
};

#endif
