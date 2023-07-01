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


#include "graphics/vga.h"
#include "graphics/videos.h"

#define PL_MPEG_IMPLEMENTATION
#include "graphics/pl_mpeg.h"

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[30][STACK_SIZE];
char* mainStack = stacks[0];

Guarded_Semaphore screen_sem(1);

VGA vga;
plm_t plm;
unsigned char rgbBuffer[320 * 200 * 3];

Application::Application() : Thread(&mainStack[STACK_SIZE]) {

}

void Application::action()
{
  kout.clear();

  vga.setMode(320, 200, 8);
  vga.fillRect(0, 0, 320, 200, 0);
  vga.loadDefaultPalette();

  /*
  for (int y = 0; y < 200; y++) {
    for (int x = 0; x < 320; x++) {
      int sX = x / 10;
      int sY = y / 10;
      int index = sX + sY * (320 / 10);
      vga.putPixel(x, y, index);
    }
  }
  cpu.halt();
  */

  plm_create_with_memory(&plm, (uint8_t*)LECTURE_1, sizeof(LECTURE_1));
  plm_set_audio_enabled(&plm, false);

  plm_frame_t *frame = nullptr;
  for (int i = 1; frame = plm_decode_video(&plm); i++) {
    plm_frame_to_rgb(frame, rgbBuffer, 320 * 3);

    // TODO: display
    for (int y = 0; y < 200; y++) {
      for (int x = 0; x < 320; x++) {
        int colorIndex = 0;
        unsigned char r = rgbBuffer[(y * 320 + x) * 3];
        unsigned char g = rgbBuffer[(y * 320 + x) * 3 + 1];
        unsigned char b = rgbBuffer[(y * 320 + x) * 3 + 2];

        /*
        if (r > 127 && g > 127 && b > 127) colorIndex = 0x3F; // white
        else if (r > g && r > b) colorIndex = 0x04; // red
        else if (g > r && g > b) colorIndex = 0x02; // green
        else if (b > r && b > g) colorIndex = 0x01; // blue
        else colorIndex = 0x00;

        vga.putPixel(x, y, colorIndex);
        */
        vga.putPixel(x, y, r, g, b);
      }
    }
  }

  Guarded_Buzzer buzz1;
  Guarded_Buzzer buzz2;
  Guarded_Buzzer buzz3;
  Guarded_Buzzer buzz4;

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

  VgaThread t18(18, &stacks[18][STACK_SIZE], &buzz1, false, 0, 0, 50, 50);
  VgaThread t19(19, &stacks[19][STACK_SIZE], &buzz2, false, 100, 0, 50, 50);
  VgaThread t20(20, &stacks[20][STACK_SIZE], &buzz3, false, 200, 0, 50, 50);

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


VgaThread::VgaThread(int threadId, void* tos, Guarded_Buzzer* buzzer, bool mainBuzz, int x, int y, int width, int height) :
  Thread(tos), id(threadId), buzzer(buzzer), mainBuzz(mainBuzz), x(x), y(y), width(width), height(height)
{
  organizer.ready(*this);
}

void VgaThread::action() {
  int index = 0;

  while (true) {
    if (mainBuzz) buzzer->reset();
    buzzer->sleep();

    vga.fillRect(x, y, width, height, index);

    index = (index + 1) % 64;
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
