/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The Application class defines the (only) application for OOStuBS.         */
/*****************************************************************************/

// GRUPPE 13!!!

/* INCLUDES */

#include "user/appl.h"
#include "device/cgastr.h"

#include "machine/cpu.h"
#include "guard/guard.h"
#include "guard/secure.h"
#include "syscall/guarded_organizer.h"
#include "thread/entrant.h"
#include "device/watch.h"
#include "syscall/guarded_semaphore.h"
#include "syscall/guarded_keyboard.h"

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[20][STACK_SIZE];
char* mainStack = stacks[0];

Guarded_Semaphore screen_sem(1);

Application::Application() : Thread(&mainStack[STACK_SIZE]) {

}

void Application::action()
{
  kout.clear();

  Guarded_Buzzer buzz1;
  Guarded_Buzzer buzz2;
  Guarded_Buzzer buzz3;

  buzz1.set(500);
  buzz2.set(1000);
  buzz3.set(2000);

  // prepare some threads
  // TestThread t1(1, &stacks[1][STACK_SIZE]);
  
  PeriodicThread t2(2, &stacks[2][STACK_SIZE], &buzz1, true);
  PeriodicThread t3(3, &stacks[3][STACK_SIZE], &buzz1, false);
  PeriodicThread t4(4, &stacks[4][STACK_SIZE], &buzz1, false);
  PeriodicThread t5(5, &stacks[5][STACK_SIZE], &buzz1, false);
  PeriodicThread t6(6, &stacks[6][STACK_SIZE], &buzz1, false);
  PeriodicThread t7(7, &stacks[7][STACK_SIZE], &buzz1, false);
  PeriodicThread t8(8, &stacks[8][STACK_SIZE], &buzz1, false);
  PeriodicThread t9(9, &stacks[9][STACK_SIZE], &buzz1, false);

  PeriodicThread t10(10, &stacks[10][STACK_SIZE], &buzz2, true);
  PeriodicThread t11(11, &stacks[11][STACK_SIZE], &buzz2, false);
  PeriodicThread t12(12, &stacks[12][STACK_SIZE], &buzz2, false);

  PeriodicThread t13(13, &stacks[13][STACK_SIZE], &buzz3, true);
  PeriodicThread t14(14, &stacks[14][STACK_SIZE], &buzz3, false);
  PeriodicThread t15(15, &stacks[15][STACK_SIZE], &buzz3, false);
  
  KeyboardThread t17(17, &stacks[17][STACK_SIZE]);

  // enable preemptive scheduling
  watch.windup();

  // exit the current execution and start the threads
  organizer.exit();

  // this code won't be executed - it's here to demonstrate that scheduler.exit() works
  {
    Secure secure;

    kout.setPos(1, 5);
    kout << "This should not be displayed";
    kout.flush();
  }
}

TestThread::TestThread(int threadId, void* tos) : Thread(tos), id(threadId)
{
  organizer.ready(*this);
}

void TestThread::action() {
  unsigned long cnt = 0;
  while (true) {
    if (this->killPtr && cnt >= 1000) {
      organizer.kill(*killPtr);
    }

    // needed for VirtualBox cause otherwise the mutexed section is much longer than the
    // non-mutexed section and the other threads would starve
    for (int i = 0; i < 100000; i++) {
      i = i + 1 - 2 + 1;
    }

    screen_sem.wait();

    kout.setPos(1, id);
    kout << "Thread #" << dec << id << ": " << dec << cnt;
    kout.flush();

    screen_sem.signal();

    // organizer.resume(); // TODO: can be used to allow fair scheduling (kinda no preemption)
    
    cnt++;
  }
}

void TestThread::setKillPtr(Thread* that) {
  this->killPtr = that;
}


PeriodicThread::PeriodicThread(int threadId, void* tos, Guarded_Buzzer* buzzer, bool mainBuzz) : Thread(tos), id(threadId), buzzer(buzzer), mainBuzz(mainBuzz) {
  organizer.ready(*this);
}

void PeriodicThread::action() {
  int cnt = 0;
  while (true) {
    if (mainBuzz) buzzer->reset();
    buzzer->sleep();

    screen_sem.wait();

    kout.setPos(1, id);
    kout << "Thread #" << dec << id << ": sleep - " << dec << ++cnt;
    kout.flush();

    screen_sem.signal();
  }
}


KeyboardThread::KeyboardThread(int threadId, void* tos) : Thread(tos), id(threadId), x(1), y(threadId) {
  organizer.ready(*this);
}

void KeyboardThread::action() {
  while (true) {
    Key key = keyboard.getKey();

    screen_sem.wait();

    kout.setPos(x, y);
    kout << key.ascii();
    kout.flush();
    kout.getPos(x, y);
    
    screen_sem.signal();
  }
}
