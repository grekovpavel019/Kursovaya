#include <stdio.h>
#include "audio.h"  

static Mix_Chunk* bounceSound = NULL;
static Mix_Chunk* hitSound = NULL;
static Mix_Chunk* scoreSound = NULL;
static Mix_Chunk* winSound = NULL;
static Mix_Music* bgMusic = NULL;
bool musicEnabled = true;

bool init_audio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize: %s\n", Mix_GetError());
        return false;
    }

    // Загрузка звуков
    bounceSound = Mix_LoadWAV("hit.wav");
    hitSound = Mix_LoadWAV("hit.wav");
    scoreSound = Mix_LoadWAV("score.wav");
    winSound = Mix_LoadWAV("win.wav");

    if (!bounceSound || !hitSound || !scoreSound || !winSound) {
        printf("Failed to load sound effects: %s\n", Mix_GetError());
        return false;
    }

    // Загрузка фоновой музыки
    bgMusic = Mix_LoadMUS("music.mp3");
    if (!bgMusic) {
        printf("Failed to load background music: %s\n", Mix_GetError());
    }

    // Запуск музыки
    if (bgMusic && musicEnabled) {
        Mix_PlayMusic(bgMusic, -1); // -1 для бесконечного повторения
    }

    return true;
}

void close_audio() {
    if (bounceSound) Mix_FreeChunk(bounceSound);
    if (hitSound) Mix_FreeChunk(hitSound);
    if (scoreSound) Mix_FreeChunk(scoreSound);
    if (winSound) Mix_FreeChunk(winSound);
    if (bgMusic) Mix_FreeMusic(bgMusic);

    Mix_CloseAudio();
}

void play_sound(int soundType) {
    if (!soundEnabled) return;

    switch (soundType) {
    case 0: Mix_PlayChannel(-1, bounceSound, 0); break;  // Отскок от стенки
    case 1: Mix_PlayChannel(-1, hitSound, 0); break;    // Удар по ракетке
    case 2: Mix_PlayChannel(-1, scoreSound, 0); break;   // Гол
    case 3: Mix_PlayChannel(-1, winSound, 0); break;     // Победа
    }
}

void toggle_sound() {
    soundEnabled = !soundEnabled;
}

void toggle_music() {
    musicEnabled = !musicEnabled;
    if (bgMusic) {
        if (musicEnabled) {
            Mix_PlayMusic(bgMusic, -1);
        }
        else {
            Mix_HaltMusic();
        }
    }
}