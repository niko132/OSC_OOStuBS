# OOStuBS Group 13

The application built on top of the OS is a simple MPEG-I decoder and player (hence the name VLC).
The MPEG-I videos are hardcoded in the header files. Currently you can choose between two files in the program:
- schirmi.mpeg
- rick.mpeg

## Core features
- a VGA driver -> switch to graphics mode
- custom made graphics-mode text (only some chars implemented!)
- a sound "driver" for the SB16 card
- a MPEG-I decoder and player

## How to run
The Makefile includes an option for speaker support:
```
make qemu-spk
```
This option just adds the following arg to the qemu executable:
```
-device sb16
```

## Usage
Once bootet, the player shows a menu where u can choose between the two video files. You select the video using the Keys `1` or `2` on your keyboard.
The video starts playing after a short time (and the audio gets more and more async xD).
The audio will just loop forever while the video will freeze at the end.
After that you can press the `ESC` key to go back to the menu or `ANY OTHER KEY` to replay the video.

## Issues
- Video and Audio will be async. The system is just too slow to play them simultaneously.
- The Audio will stop working after the first video was played.
- The Audio quality is very bad and noisy. (The actual SB16 high quality mode was not working)
- When using more than one thread at a time, the memory of the threads gets disrupted. Things work perfectly fine with one thread. (Thats also why the video player only uses one thread...)

## Decoder
The MPEG-I decoder was taken from https://github.com/phoboslab/pl_mpeg (thanks to `phoboslab`!) and modified so that it works with OOStuBS.
