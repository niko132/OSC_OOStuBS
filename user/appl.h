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

#include "syscall/thread.h"

class Application : public Thread
{

public:
	Application (const Application &copy) = delete; // prevent copying
  	Application();

	void action ();
};

class TestThread : public Thread
{
public:
	int id;
	Thread* killPtr = nullptr;

	TestThread(int threadId, void* tos);

	void action();
	void setKillPtr(Thread* that);
};

#endif
