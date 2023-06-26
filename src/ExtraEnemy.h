#pragma once
#include <iostream>
#include <vector>
#include <chrono>

#include "CMovable.h"

/**
 * @brief Child class of CMovable
 * 
 */
class ExtraEnemy: public CMovable{
    const double m_move_cycle;
    std::chrono::steady_clock::time_point m_pitch;

    void move_extra(); //called by only update method.

public:
    ExtraEnemy(SDL_Rect pos, double move_cycle);

    void init(SDL_Renderer* renderer) override;

    // update positions
    std::vector<std::shared_ptr<CObj>> update() override;

    //collisions
    void collision(CObj& other) override;
    void collision( ExtraEnemy&) override;
    void collision( Wall& ) override;
    void collision( Player& ) override;
    void collision( BreakableWall& ) override;
    void collision( Bomb& ) override;
    void collision( Blast&) override;
    void collision( Item&) override;
};