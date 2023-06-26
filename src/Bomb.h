#pragma once
#include <iostream>
#include <vector>
#include <chrono>

#include "CObj.h"
#include "dir.h"

/**
 * @brief Bomb class.
 *     This object destroyed after 'm_time_to_bomb' seconds from created.
 *          And then it is trigger to create Blast objects.
 *          in this class set blast life time to m_time_to_bomb/5 seconds.
 *  
 *  Bomb Object Can be Movable in future implementation.
 */
class Bomb: public CObj{
    const double m_time_to_bomb;
    std::chrono::steady_clock::time_point m_life_start_time;// time when this object born, to exprode after 'm_time_to_bomb' seconds
    Player* m_owner;//player who made this blast. to evaluate point 
    bool m_unlimited_blast;// if this true, there are blast with direction to unlimitedly blast.
    bool m_remote_controlled; //tag for remote controll.
public:
    Bomb(const SDL_Rect& pos, Player* owner, bool unlimited_blast, bool remote_control, double time_to_bomb);

    void init (SDL_Renderer* renderer) override;

    // when it exprode, make child corresponding direction, given by 'm_blast_life_time'
    std::vector<std::shared_ptr<CObj>> update() override;

     //collisions
    void collision(CObj& other) override {other.collision(*this);}
    void collision( ExtraEnemy& ) override {}
    void collision( Wall&) override {}
    void collision( Player&) override {}
    void collision( BreakableWall&) override {}
    void collision( Bomb&) override {}
    void collision(Blast&) override {}
    void collision(Item&) override {}
};
