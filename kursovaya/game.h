#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

// Константы игры
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 100
#define BALL_SIZE 15
#define WIN_SCORE 11
#define PADDLE_SPEED 8
#define BALL_SPEED 5
#define MAX_BALL_SPEED 15

// Состояния игры
typedef enum {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
} GameState;

// Структура мяча
typedef struct {
    float x, y;
    float velX, velY;
    int size;
    float speed;
} Ball;

// Структура ракетки
typedef struct {
    float x, y;
    float velY;
    int width, height;
    int score;
} Paddle;

// Глобальные переменные игры
extern GameState gameState;
extern Ball ball;
extern Paddle paddle1, paddle2;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern bool isRunning;
extern bool soundEnabled;
extern bool musicEnabled;

// Функции инициализации
bool init_game();
void close_game();
bool init_renderer();
void close_renderer();
bool init_audio();
void close_audio();

// Игровые функции
void reset_ball();
void reset_paddles();
void update_game();
void check_collisions();
void check_win_condition();

// Функции рендеринга
void render_game();
void render_menu();
void render_pause();
void render_game_over();

// Функции ввода
void handle_events();
void handle_keyboard();

// Аудио функции
void play_sound(int soundType);
void toggle_sound();
void toggle_music();

#endif