#include "sound/player.h"

Watch soundWatch(1000 * 1000 / 8000);

void ProgramPIT() {
    soundWatch.windup();
}

unsigned char* WAVFile = (unsigned char*)PCM_SAMPLE_1;
unsigned int WAVSize = sizeof(PCM_SAMPLE_1);