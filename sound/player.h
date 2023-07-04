#ifndef __PLAYER_include__
#define __PLAYER_include__

#include "sound/pcm_sample.h"
#include "device/watch.h"


extern unsigned char* WAVFile;
extern unsigned int WAVSize;

extern "C" void PlayWAV(unsigned char* pcm);
extern "C" void ir_sound();
extern "C" void ProgramPIT();

extern Watch soundWatch;

void ProgramPIT();

#endif
