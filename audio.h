#ifndef AUDIO_H
#define AUDIO_H

#include "game.h"

bool init_audio();
void close_audio();
void play_sound(int soundType);
void toggle_sound();
void toggle_music();

#endif