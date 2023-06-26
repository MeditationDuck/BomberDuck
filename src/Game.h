#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>


#include "GameBoard.h"
#include "GameRecords.h"

/**
 * @brief Game class
 *        Manage SDL Interface and user input.
 */
class Game{
    static constexpr  SDL_Color white = {255, 255, 255,255};
    static constexpr SDL_Color red = {255, 0, 0,255};

    enum GameState {PLAY, EXIT};

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font * m_font;
    SDL_Event m_event;

    int m_screenWidth;
    int m_screenHeight;
    int m_cnt;
    int m_state;
    GameBoard m_gameboard;
    GameRecords m_gamerecods;

    // function to draw text.
    void drawText( const std::string& text, SDL_Color color, int x, int y);

    // User select game mode and load maps.
    bool ChooseGameMode();

    // call 'Gameboard::LoadMap' since objects managed by Gameboard,
    // and dicide screen size by map.
    void LoadMap();

    // Init SDL and create window on screen.
    void init();

    //show to player the result of game, and show top 5 recocrd of the game from file and Player results.
    bool ConcludeGame();
 
    //render game objects and draw current points and best records point.
    void render();

    // handle user input events.
    void HandleEvent();
    
    //clean the game.
    void clean();

public:
    Game();
    // called by main function.
    void run();
};
