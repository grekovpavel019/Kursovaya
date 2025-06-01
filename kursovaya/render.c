#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "render.h"

TTF_Font* font = NULL;
SDL_Color textColor = { 255, 255, 255, 255 };
SDL_Color buttonColor = { 70, 70, 70, 255 };
SDL_Color buttonHoverColor = { 100, 100, 100, 255 };

SDL_Texture* playTexture = NULL;
SDL_Texture* exitTexture = NULL;
SDL_Texture* titleTexture = NULL;
SDL_Texture* soundOnTexture = NULL;
SDL_Texture* soundOffTexture = NULL;
SDL_Texture* musicOnTexture = NULL;
SDL_Texture* musicOffTexture = NULL;
SDL_Texture* backgroundTexture = NULL;

SDL_Rect playButtonRect;
SDL_Rect exitButtonRect;
SDL_Rect soundButtonRect;
SDL_Rect musicButtonRect;
SDL_Rect continueButtonRect;
SDL_Rect menuButtonRect;

bool init_renderer() {

    // Загрузка фонового изображения
    SDL_Surface* bgSurface = SDL_LoadBMP("background.bmp");
    if (!bgSurface) {
        printf("Failed to load background image: %s\n", SDL_GetError());
        // Можно продолжить без фонового изображения
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
        if (!backgroundTexture) {
            printf("Failed to create texture from surface: %s\n", SDL_GetError());
        }
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont("Tinos-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }

    // Создаем текстуры для меню
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Play", textColor);
    playTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Exit", textColor);
    exitTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Table tennis", textColor);
    titleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Sound ON", textColor);
    soundOnTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Sound OFF", textColor);
    soundOffTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Music ON", textColor);
    musicOnTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(font, "Music OFF", textColor);
    musicOffTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Инициализация прямоугольников кнопок
    playButtonRect = (SDL_Rect){ SCREEN_WIDTH / 2 - 100, 200, 200, 50 };
    exitButtonRect = (SDL_Rect){ SCREEN_WIDTH / 2 - 100, 270, 200, 50 };
    soundButtonRect = (SDL_Rect){ SCREEN_WIDTH - 220, 20, 100, 30 };
    musicButtonRect = (SDL_Rect){ SCREEN_WIDTH - 110, 20, 100, 30 };
    continueButtonRect = (SDL_Rect){ SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 50 };
    menuButtonRect = (SDL_Rect){ SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 70, 200, 50 };

    return true;
}

void close_renderer() {
    if (playTexture) SDL_DestroyTexture(playTexture);
    if (exitTexture) SDL_DestroyTexture(exitTexture);
    if (titleTexture) SDL_DestroyTexture(titleTexture);
    if (soundOnTexture) SDL_DestroyTexture(soundOnTexture);
    if (soundOffTexture) SDL_DestroyTexture(soundOffTexture);
    if (musicOnTexture) SDL_DestroyTexture(musicOnTexture);
    if (musicOffTexture) SDL_DestroyTexture(musicOffTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (font) TTF_CloseFont(font);
}

void render_game() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Отрисовка фона (если загружен)
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
    else {
        // Градиентный фон как запасной вариант
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 20 + (y * 15 / SCREEN_HEIGHT);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
        }
    }

    // Фоновый градиент
    /*for (int y = 0; y < SCREEN_HEIGHT; y++) {
        Uint8 r = 0;
        Uint8 g = 0;
        Uint8 b = 20 + (y * 15 / SCREEN_HEIGHT);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }*/

    // Центральная линия
    SDL_SetRenderDrawColor(renderer, 100, 255, 255, 150);
    for (int y = 0; y < SCREEN_HEIGHT; y += 20) {
        SDL_Rect line = { SCREEN_WIDTH / 2 - 2, y, 4, 10 };
        SDL_RenderFillRect(renderer, &line);
    }

    // Мяч и его свечение
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect ballRect = { (int)ball.x, (int)ball.y, ball.size, ball.size };
    SDL_RenderFillRect(renderer, &ballRect);
    SDL_SetRenderDrawColor(renderer, 100, 255, 255, 100);
    for (int i = 1; i <= 3; i++) {
        SDL_Rect glowRect = {
            (int)ball.x - i,
            (int)ball.y - i,
            ball.size + i * 2,
            ball.size + i * 2
        };
        SDL_RenderDrawRect(renderer, &glowRect);
    }

    // Ракетка игрока 1 (красная с градиентом)
    for (int y = 0; y < paddle1.height; y++) {
        Uint8 r = 255;
        Uint8 g = 50 + y * 100 / paddle1.height;
        Uint8 b = 50 + y * 100 / paddle1.height;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer,
            paddle1.x,
            paddle1.y + y,
            paddle1.x + paddle1.width,
            paddle1.y + y);
    }

    // Ракетка игрока 2 (синяя с градиентом)
    for (int y = 0; y < paddle2.height; y++) {
        Uint8 r = 50 + y * 100 / paddle2.height;
        Uint8 g = 50 + y * 100 / paddle2.height;
        Uint8 b = 255;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer,
            paddle2.x,
            paddle2.y + y,
            paddle2.x + paddle2.width,
            paddle2.y + y);
    }

    // Отображение счёта
    char scoreText[50];
    sprintf(scoreText, "%d - %d", paddle1.score, paddle2.score);
    SDL_Color shadowColor = { 0, 0, 0, 255 };
    SDL_Surface* shadowSurface = TTF_RenderText_Solid(font, scoreText, shadowColor);
    SDL_Texture* shadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurface);
    SDL_Rect shadowRect = { SCREEN_WIDTH / 2 - 28, 22, 60, 30 };
    SDL_RenderCopy(renderer, shadowTexture, NULL, &shadowRect);

    SDL_Color neonTextColor = { 100, 255, 255, 255 };
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, neonTextColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 30, 20, 60, 30 };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_DestroyTexture(shadowTexture);
    SDL_FreeSurface(shadowSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scoreSurface);

    // Кнопки звука и музыки
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 200);
    SDL_RenderFillRect(renderer, &soundButtonRect);
    SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &soundButtonRect);
    SDL_RenderCopy(renderer, soundEnabled ? soundOnTexture : soundOffTexture, NULL, &soundButtonRect);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 200);
    SDL_RenderFillRect(renderer, &musicButtonRect);
    SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &musicButtonRect);
    SDL_RenderCopy(renderer, musicEnabled ? musicOnTexture : musicOffTexture, NULL, &musicButtonRect);

    // Пауза
    if (gameState == PAUSED) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Color pauseColor = { 100, 255, 255, 255 };
        SDL_Surface* pauseSurface = TTF_RenderText_Solid(font, "PAUSE", pauseColor);
        SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        SDL_Rect pauseRect = { SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 100, 140, 50 };
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_Color continueColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & continueButtonRect)
            ? buttonHoverColor : buttonColor;
        SDL_SetRenderDrawColor(renderer, continueColor.r, continueColor.g, continueColor.b, 255);
        SDL_RenderFillRect(renderer, &continueButtonRect);

        SDL_Color menuColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & menuButtonRect)
            ? buttonHoverColor : buttonColor;
        SDL_SetRenderDrawColor(renderer, menuColor.r, menuColor.g, menuColor.b, 255);
        SDL_RenderFillRect(renderer, &menuButtonRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &continueButtonRect);
        SDL_RenderDrawRect(renderer, &menuButtonRect);

        SDL_Surface* continueSurface = TTF_RenderText_Solid(font, "Continue", pauseColor);
        SDL_Texture* continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
        SDL_RenderCopy(renderer, continueTexture, NULL, &continueButtonRect);

        SDL_Surface* menuSurface = TTF_RenderText_Solid(font, "Main menu", pauseColor);
        SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
        SDL_RenderCopy(renderer, menuTexture, NULL, &menuButtonRect);

        SDL_DestroyTexture(pauseTexture);
        SDL_FreeSurface(pauseSurface);
        SDL_DestroyTexture(continueTexture);
        SDL_FreeSurface(continueSurface);
        SDL_DestroyTexture(menuTexture);
        SDL_FreeSurface(menuSurface);
    }

    SDL_RenderPresent(renderer);
}


void render_menu() {

    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Отрисовка фона (если загружен)
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
        SDL_RenderFillRect(renderer, NULL);
    }

    // Очистка экрана
    /*SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
    SDL_RenderClear(renderer);*/

    // Отрисовка заголовка
    SDL_Rect titleRect = { SCREEN_WIDTH / 2 - 150, 100, 300, 60 };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    // Получаем позицию мыши для эффекта наведения
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Отрисовка кнопок с эффектом наведения
    // Кнопка "Играть"
    SDL_Color playColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & playButtonRect) ? buttonHoverColor : buttonColor;
    SDL_SetRenderDrawColor(renderer, playColor.r, playColor.g, playColor.b, 255);
    SDL_RenderFillRect(renderer, &playButtonRect);

    // Кнопка "Выход"
    SDL_Color exitColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & exitButtonRect) ? buttonHoverColor : buttonColor;
    SDL_SetRenderDrawColor(renderer, exitColor.r, exitColor.g, exitColor.b, 255);
    SDL_RenderFillRect(renderer, &exitButtonRect);

    // Рамки для кнопок
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &playButtonRect);
    SDL_RenderDrawRect(renderer, &exitButtonRect);

    // Текст на кнопках
    SDL_RenderCopy(renderer, playTexture, NULL, &playButtonRect);
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitButtonRect);

    // Кнопка звука
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
    SDL_RenderFillRect(renderer, &soundButtonRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &soundButtonRect);
    SDL_RenderCopy(renderer, soundEnabled ? soundOnTexture : soundOffTexture, NULL, &soundButtonRect);

    // Кнопка музыки
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
    SDL_RenderFillRect(renderer, &musicButtonRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &musicButtonRect);
    SDL_RenderCopy(renderer, musicEnabled ? musicOnTexture : musicOffTexture, NULL, &musicButtonRect);

    SDL_RenderPresent(renderer);
}

void render_pause() {
    // Сначала рисуем игру
    render_game();

    // Затем полупрозрачный overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    // Текст паузы
    /*SDL_Surface* pauseSurface = TTF_RenderText_Solid(font, u8"Pause", textColor);
    SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
    SDL_Rect pauseRect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 60, 100, 30 };
    SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);*/

    // Кнопки в меню паузы
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Rect continueRect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 50 };
    SDL_Rect menuRect = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 70, 200, 50 };

    // Кнопка "Продолжить"
    SDL_Color continueColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & continueRect) ? buttonHoverColor : buttonColor;
    SDL_SetRenderDrawColor(renderer, continueColor.r, continueColor.g, continueColor.b, 255);
    SDL_RenderFillRect(renderer, &continueRect);

    // Кнопка "В меню"
    SDL_Color menuColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & menuRect) ? buttonHoverColor : buttonColor;
    SDL_SetRenderDrawColor(renderer, menuColor.r, menuColor.g, menuColor.b, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    // Рамки кнопок
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &continueRect);
    SDL_RenderDrawRect(renderer, &menuRect);

    // Текст для кнопок
    SDL_Surface* continueSurface = TTF_RenderText_Solid(font, u8"Continue", textColor);
    SDL_Texture* continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
    SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);

    SDL_Surface* menuSurface = TTF_RenderText_Solid(font, u8"Main menu", textColor);
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_RenderCopy(renderer, menuTexture, NULL, &menuRect);

    // Освобождение ресурсов
    /*SDL_DestroyTexture(pauseTexture);
    SDL_FreeSurface(pauseSurface);*/
    SDL_DestroyTexture(continueTexture);
    SDL_FreeSurface(continueSurface);
    SDL_DestroyTexture(menuTexture);
    SDL_FreeSurface(menuSurface);

    SDL_RenderPresent(renderer);
}

void render_game_over() {
    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
    SDL_RenderClear(renderer);

    // Определяем победителя
    const char* winnerText = paddle1.score > paddle2.score ? u8"Player 1 win!" : u8"Player 2 win!";

    // Рендерим текст победителя
    SDL_Surface* winnerSurface = TTF_RenderText_Solid(font, winnerText, textColor);
    SDL_Texture* winnerTexture = SDL_CreateTextureFromSurface(renderer, winnerSurface);
    SDL_Rect winnerRect = { SCREEN_WIDTH / 2 - 150, 150, 300, 40 };
    SDL_RenderCopy(renderer, winnerTexture, NULL, &winnerRect);

    // Рендерим счет
    char scoreText[50];
    sprintf(scoreText, u8"Score: %d - %d", paddle1.score, paddle2.score);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Rect scoreRect = { SCREEN_WIDTH / 2 - 80, 220, 160, 30 };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    // Получаем позицию мыши
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Кнопка возврата в меню (используем глобальный menuButtonRect)
    menuButtonRect = (SDL_Rect){ SCREEN_WIDTH / 2 - 100, 300, 200, 50 }; // Обновляем позицию
    SDL_Color menuColor = SDL_PointInRect(&(SDL_Point) { mouseX, mouseY }, & menuButtonRect) ? buttonHoverColor : buttonColor;

    SDL_SetRenderDrawColor(renderer, menuColor.r, menuColor.g, menuColor.b, 255);
    SDL_RenderFillRect(renderer, &menuButtonRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &menuButtonRect);

    SDL_Surface* menuSurface = TTF_RenderText_Solid(font, u8"Main menu", textColor);
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_RenderCopy(renderer, menuTexture, NULL, &menuButtonRect);

    // Освобождение ресурсов
    SDL_DestroyTexture(winnerTexture);
    SDL_FreeSurface(winnerSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(menuTexture);
    SDL_FreeSurface(menuSurface);

    SDL_RenderPresent(renderer);
}