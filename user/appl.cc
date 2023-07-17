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
#include "graphics/rick.h"

#define PL_MPEG_IMPLEMENTATION
#include "graphics/pl_mpeg.h"

#include "sound/sb16.h"
#include "machine/pic.h"

#include "graphics/schirmi.h"

/* GLOBAL VARIABLES */

extern CGA_Stream kout;

const int STACK_SIZE = 64 * 1024; // 64kB

char stacks[30][STACK_SIZE];
char* mainStack = stacks[0];

Guarded_Semaphore screen_sem(1);

VGA vga;
unsigned char rgbBuffer[320 * 200 * 3];

Application::Application() : Thread(&mainStack[STACK_SIZE]) {

}

void app_on_video(plm_t *plm, plm_frame_t *frame, void *obj) {
  plm_frame_to_rgb(frame, rgbBuffer, 320 * 3);

  for (int j = 0; j < 320 * 200; j += 4) {
    unsigned char r = rgbBuffer[j * 3];
    unsigned char g = rgbBuffer[j * 3 + 1];
    unsigned char b = rgbBuffer[j * 3 + 2];

    vga.putPixelFast(r, g, b);
  }
}

unsigned char sample_buf[1152];
void app_on_audio(plm_t *plm, plm_samples_t *samples, void *obj) {
  for (int j = 0; j < 1152; j++) {
    sample_buf[j] = 127 + 127 * samples->left[j];
  }

  sb16.addSamples(sample_buf, 1152);
}

void Application::action()
{
  kout.clear();

  // enable preemptive scheduling
  watch.windup();

  sb16.windup();
  Key key;

  vga.setMode(320, 200, 8);
  vga.fillRect(0, 0, 320, 200, 0);
  vga.loadDefaultPalette();

  while (true) {
    vga.fillRect(0, 0, 320, 200, 0);
    vga.writeText(150, 10, (unsigned char*)"VLC");
    vga.writeText(10, 80, (unsigned char*)"1 - schirmi.mpeg");
    vga.writeText(10, 94, (unsigned char*)"2 - rick.mpeg");
    key = keyboard.getKey();

    plm_t plm;
    if (key.ascii() == '1') {
      plm_create_with_memory(&plm, (uint8_t*)SCHIRMI_1, sizeof(SCHIRMI_1));
    } else if (key.ascii() == '2') {
      plm_create_with_memory(&plm, (uint8_t*)RICK_1, sizeof(RICK_1));
    } else {
      continue;
    }

    vga.fillRect(0, 0, 320, 200, vga.getColorIndex(255, 80, 0));

    // prepare decoder
    plm_set_loop(&plm, true);
    plm_set_audio_enabled(&plm, true);
    plm_set_audio_stream(&plm, 0);

    // prepare audio
    unsigned char sample_buf[1764];
    plm_samples_t* samples = nullptr;
    const int reduction = 2;
    while ((samples = plm_decode_audio(&plm)) != nullptr) {
      unsigned int count = 0;
      for (int i = 0; i < 1764; i += reduction) {
        sample_buf[count++] = 127 + 127 * samples->left[i];
      }
      sb16.addSamples(sample_buf, count);
    }

    // start audio
    sb16.reset();
    sb16.programm();

    // start video
    // needed to guarantee at most 25fps (maybe only useful on real hw)
    Guarded_Buzzer frameBuzz;
    frameBuzz.set(25);

    while (true) {
      unsigned int video_buf_index = 0;
      plm_frame_t* frame = nullptr;
      while ((frame = plm_decode_video(&plm)) != nullptr) {
        frameBuzz.reset();
        plm_frame_to_rgb(frame, rgbBuffer, 320 * 3);

        for (int j = 0; j < 320 * 200; j++) {
          unsigned char r = rgbBuffer[j * 3];
          unsigned char g = rgbBuffer[j * 3 + 1];
          unsigned char b = rgbBuffer[j * 3 + 2];
          vga.putPixelFast(r, g, b);
        }
        vga.resetFastMode();
        frameBuzz.wait();
      }

      Key key = keyboard.getKey();
      if (key.scancode() == 1) break; // ESC
      plm_rewind(&plm);
    }
    
    // sb16.stop();
    sb16.resetBuffer();
  }

  // exit the current execution and start the threads
  organizer.exit();
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