#include <stdio.h>
#include "input.h"
#include "render.h"
#include "audio.h"

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Проверка кнопки звука
                if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & soundButtonRect)) {
                    toggle_sound();
                    break;
                }

                // Проверка кнопки музыки
                if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & musicButtonRect)) {
                    toggle_music();
                    break;
                }

                switch (gameState) {
                case MENU:
                    if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & playButtonRect)) {
                        gameState = PLAYING;
                        reset_ball();
                        reset_paddles();
                    }
                    else if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & exitButtonRect)) {
                        isRunning = false;
                    }
                    break;

                case PAUSED:
                    if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & continueButtonRect)) {
                        gameState = PLAYING;
                    }
                    else if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & menuButtonRect)) {
                        gameState = MENU;
                    }
                    break;

                case GAME_OVER:
                    if (SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & menuButtonRect)) {
                        gameState = MENU;
                    }
                    break;

                case PLAYING:
                default:
                    break;
                }
            }
            break;

        case SDL_KEYDOWN:
            handle_key_down(&event.key);
            break;

        case SDL_KEYUP:
            handle_key_up(&event.key);
            break;
        }
    }
}

static void handle_key_down(SDL_KeyboardEvent* key) {
    static Uint32 lastKeyPressTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    // Защита от слишком частых нажатий (антидребезг)
    if (currentTime - lastKeyPressTime < 300) { // 300ms задержка
        return;
    }
    lastKeyPressTime = currentTime;

    switch (gameState) {
    case PLAYING:
        switch (key->keysym.sym) {
        case SDLK_w: paddle1.velY = -PADDLE_SPEED; break;
        case SDLK_s: paddle1.velY = PADDLE_SPEED; break;
        case SDLK_UP: paddle2.velY = -PADDLE_SPEED; break;
        case SDLK_DOWN: paddle2.velY = PADDLE_SPEED; break;
        case SDLK_ESCAPE:
            gameState = PAUSED;
            break;
        }
        break;

    case MENU:
        switch (key->keysym.sym) {
        case SDLK_SPACE:
        case SDLK_RETURN:
            gameState = PLAYING;
            reset_ball();
            reset_paddles();
            break;
        case SDLK_ESCAPE:
            isRunning = false;
            break;
        }
        break;

    case PAUSED:
        switch (key->keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_RETURN:
            gameState = PLAYING;
            break;
        case SDLK_m:
            gameState = MENU;
            reset_ball();
            reset_paddles();
            break;
        }
        break;

    case GAME_OVER:
        switch (key->keysym.sym) {
        case SDLK_RETURN:
        case SDLK_SPACE:
        case SDLK_m:
            gameState = MENU;
            reset_ball();
            reset_paddles();
            break;
        case SDLK_ESCAPE:
            isRunning = false;
            break;
        }
        break;
    }
}

static void handle_key_up(SDL_KeyboardEvent* key) {
    if (gameState == PLAYING) {
        switch (key->keysym.sym) {
        case SDLK_w:
        case SDLK_s: paddle1.velY = 0; break;
        case SDLK_UP:
        case SDLK_DOWN: paddle2.velY = 0; break;
        }
    }
}

void handle_keyboard() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (gameState == PLAYING) {
        paddle1.velY = 0;
        paddle2.velY = 0;

        if (keystates[SDL_SCANCODE_W]) paddle1.velY = -PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_S]) paddle1.velY = PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_UP]) paddle2.velY = -PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_DOWN]) paddle2.velY = PADDLE_SPEED;
    }
}