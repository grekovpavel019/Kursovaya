#ifndef RENDER_H
#define RENDER_H

#include "game.h"

// Объявляем структуры кнопок как extern во избежание ошибок линковки
extern SDL_Rect playButtonRect;
extern SDL_Rect exitButtonRect;
extern SDL_Rect soundButtonRect;
extern SDL_Rect musicButtonRect;
extern SDL_Rect continueButtonRect;
extern SDL_Rect menuButtonRect;
extern SDL_Texture* backgroundTexture;

bool init_renderer();
void close_renderer();
void render_game();
void render_menu();
void render_pause();
void render_game_over();

#endif