#include "sound/sound.h"

#include "sound/pcm_sample.h"

#include "device/cgastr.h"

void Sound::play_sound(unsigned int frequency) {
    unsigned int div = 1193180 / frequency;

    ctrl_port.outb(0xb6);
    channel2_port.outb((unsigned char)(div));
    channel2_port.outb((unsigned char)(div >> 8));

    unsigned char tmp = kc_port.inb();
    if (tmp != (tmp | 3)) kc_port.outb(tmp | 3);
}

void Sound::attach_to_timer() {
    unsigned char tmp = kc_port.inb();
    kc_port.outb(tmp | 3);

    ctrl_port.outb(0xb0);
    channel2_port.outb(0x00); // zero the ports
    channel2_port.outb(0x00); // zero the ports
}

int test = 0;
unsigned int test_freq = 100;

#include "device/cgastr.h"
#include "machine/cpu.h"

void Sound::play_buffer() {
    unsigned char byte = PCM_SAMPLE_1[currentIndex++];
    if (currentIndex >= sizeof(PCM_SAMPLE_1)) currentIndex = 0;

    unsigned char val = 53 * byte / 255;

    ctrl_port.outb(0x96); // single interrupt on channel2

    // low byte then high byte
    // let the outb() function handle the masking

    // channel2_port.outb(byte);
    // channel2_port.outb(0x00);

    // channel2_port.outb(test_freq & 0xff);
    // channel2_port.outb((test_freq >> 8) & 0xff);

    channel2_port.outb(val); // only write lower byte

    // kout << "test: " << dec << (int)byte << endl;
}

Sound sound;