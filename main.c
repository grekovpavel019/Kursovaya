#include <stdio.h>
#include <locale.h>
#include "game.h"
#include "input.h"
#include "render.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    // Инициализация игры
    if(!init_game()) {
        printf("Failed to initialize game!\n");
        return 1;
    }
    
    // Основной игровой цикл
    while(isRunning) {
        // Обработка ввода
        handle_events();
        handle_keyboard();
        
        // Обновление состояния игры
        update_game();
        
        // Отрисовка
        switch(gameState) {
            case MENU: render_menu(); break;
            case PLAYING: render_game(); break;
            case PAUSED: render_pause(); break;
            case GAME_OVER: render_game_over(); break;
        }
        
        // Ограничение FPS
        SDL_Delay(16); // ~60 FPS
    }
    
    // Завершение игры
    close_game();
    return 0;
}