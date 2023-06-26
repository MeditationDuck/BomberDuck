#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <algorithm>
#include <random>
#include <memory>
#include <fstream>
#include <chrono>

#include "CObj.h"
#include "CMovable.h"
#include "ExtraEnemy.h"
#include "Wall.h"
#include "BreakableWall.h"
#include "Player.h"
#include "Bomb.h"
#include "Blast.h"
#include "Item.h"
#include "dir.h"
#include "Mode.h"

/**
 * @brief Game Board class.
 * 
 *   This class manage every object as vector of pointer of CObj
 */

class GameBoard{
    const int m_step; //length of side of Gameboard cell.
    std::vector<std::shared_ptr<CObj>>m_objs; // all game objects store in here.
    std::shared_ptr<Player> keyboard_player1;
    std::shared_ptr<Player> keyboard_player2;
    Mode m_playmode; // current playmode i.e. SinglePlayer or MultiPlayer
    int m_board_width;
    int m_board_height;
    std::vector<std::pair<int, int>>m_free; // for initialize player and enemies.
    std::mt19937 m_random_generator;// random generator

    // initialization rendering for all game object.
    void render_init(SDL_Renderer* renderer);


    // For initial board. Player will placed that apart from other player or enemies.
    void remove_neighbor(const std::pair<int,int>& target);

    // adding ExtraEnemies on the board randomly. for num number of enemy.
    void addEnemies(int num, const double& enemy_move_cycle);

    // Adding player on the board.
    void addPlayer(int x, int y, int texture_type);

    void addWall(int x, int y);

    void addItem(int x, int y);

    void addBreakableWall(int x, int y);

    void addExtraEnemy(int x, int y, const double& move_cycle);
    
    // delete all m_alive is false of objects.
    void clear_objs();





public:

    GameBoard(int step = 64, const Mode& mode = Mode::SinglePlayer);

    // return game board cell size.
    int step() const;

    void Setplaymode(const Mode& playmode);

    Mode playmode() const;

    // clean shared_ptr of object for next game
    void clean();

    //load game map and return width, height of game board
    std::pair<int, int> LoadMap(const std::string& map_path);

    //get one or two players got point
    std::pair<int, int> getPlayerPoints() const;
    
    // adding enemies randomly and init texture for each game object
    void init(SDL_Renderer* renderer);

    // update all game objects
    //when game is end, return false
    bool update(SDL_Renderer* renderer);

    // render every game object
    void render(SDL_Renderer* renderer);

    // move player from keyboard state.
    void player_control_by_keyboard(const Uint8* state);
};
