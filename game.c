#include <stdio.h>
#include "game.h"
#include "input.h"
#include "render.h"
#include "audio.h"

// Глобальные переменные
GameState gameState = MENU;
Ball ball;
Paddle paddle1, paddle2;
SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
bool isRunning = true;
bool soundEnabled = true;

// Инициализация игры
bool init_game() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow("Настольный теннис", 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
    if(!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Инициализация подсистем
    if(!init_renderer()) {
        printf("Renderer init failed\n");
        return false;
    }
    
    if(!init_audio()) {
        printf("Audio init failed\n");
        return false;
    }
    
    // Инициализация игровых объектов
    reset_ball();
    reset_paddles();
    
    return true;
}

// Сброс мяча
void reset_ball() {
    ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
    ball.size = BALL_SIZE;
    ball.speed = BALL_SPEED;
    
    // Случайное направление
    int direction = rand() % 2 == 0 ? 1 : -1;
    ball.velX = direction * ball.speed;
    ball.velY = (rand() % 3 - 1) * 2.0f; // -2, 0 или 2
}

// Сброс ракеток
void reset_paddles() {
    paddle1.x = 20;
    paddle1.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle1.width = PADDLE_WIDTH;
    paddle1.height = PADDLE_HEIGHT;
    paddle1.velY = 0;
    paddle1.score = 0;
    
    paddle2.x = SCREEN_WIDTH - 20 - PADDLE_WIDTH;
    paddle2.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle2.width = PADDLE_WIDTH;
    paddle2.height = PADDLE_HEIGHT;
    paddle2.velY = 0;
    paddle2.score = 0;
}

// Обновление игры
void update_game() {
    if(gameState != PLAYING) return;
    
    // Движение мяча
    ball.x += ball.velX;
    ball.y += ball.velY;
    
    // Движение ракеток
    paddle1.y += paddle1.velY;
    paddle2.y += paddle2.velY;
    
    // Ограничение движения ракеток
    if(paddle1.y < 0) paddle1.y = 0;
    if(paddle1.y > SCREEN_HEIGHT - paddle1.height) paddle1.y = SCREEN_HEIGHT - paddle1.height;
    
    if(paddle2.y < 0) paddle2.y = 0;
    if(paddle2.y > SCREEN_HEIGHT - paddle2.height) paddle2.y = SCREEN_HEIGHT - paddle2.height;
    
    // Проверка столкновений
    check_collisions();
    
    // Проверка условия победы
    check_win_condition();
}

// Проверка столкновений
void check_collisions() {
    // Столкновение с верхом и низом
    if(ball.y <= 0 || ball.y >= SCREEN_HEIGHT - ball.size) {
        ball.velY = -ball.velY;
        play_sound(0); // Звук отскока
    }
    
    // Столкновение с левой ракеткой
    if(ball.x <= paddle1.x + paddle1.width &&
       ball.x >= paddle1.x &&
       ball.y + ball.size >= paddle1.y &&
       ball.y <= paddle1.y + paddle1.height) {
        
        // Угол отскока зависит от места удара
        float hitPos = (ball.y - paddle1.y) / paddle1.height - 0.5f;
        ball.velX = ball.speed;
        ball.velY = hitPos * ball.speed * 2;
        
        play_sound(1); // Звук удара по ракетке
        
        // Увеличиваем скорость мяча
        if(ball.speed < MAX_BALL_SPEED) {
            ball.speed += 0.2f;
        }
    }
    
    // Столкновение с правой ракеткой
    if(ball.x + ball.size >= paddle2.x &&
       ball.x + ball.size <= paddle2.x + paddle2.width &&
       ball.y + ball.size >= paddle2.y &&
       ball.y <= paddle2.y + paddle2.height) {
        
        float hitPos = (ball.y - paddle2.y) / paddle2.height - 0.5f;
        ball.velX = -ball.speed;
        ball.velY = hitPos * ball.speed * 2;
        
        play_sound(1);
        
        if(ball.speed < MAX_BALL_SPEED) {
            ball.speed += 0.2f;
        }
    }
    
    // Выход за границы (гол)
    if(ball.x < 0) {
        paddle2.score++;
        reset_ball();
        play_sound(2); // Звук гола
    }
    if(ball.x > SCREEN_WIDTH) {
        paddle1.score++;
        reset_ball();
        play_sound(2);
    }
}

// Проверка условия победы
void check_win_condition() {
    if(paddle1.score >= WIN_SCORE || paddle2.score >= WIN_SCORE) {
        gameState = GAME_OVER;
        play_sound(3); // Звук победы
    }
}

// Закрытие игры
void close_game() {
    close_audio();
    close_renderer();
    
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}