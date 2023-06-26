#pragma once
#include <iostream>
#include <vector>
#include <chrono>

#include "CObj.h"
#include "dir.h"

/**
 * @brief Blast class 
 *     This object destroyed after 'm_blast_life_time' second from created.
 */
class Blast: public CObj{
    const double m_blast_life_time;
    std::chrono::steady_clock::time_point m_life_start_time; // time when this object born, to end of exprode after 'm_blast_life_time' seconds
    Player* m_owner;//player who made this blast. to evaluate point 
    dir m_blast_direction;// for unlimited_fire item.
public:
    Blast(const SDL_Rect& pos, Player* owner, dir blast_direction, double blast_life_time);

    void init (SDL_Renderer* renderer) override;
    
    //when collide with some object, owner got point.
    //owner is managed by Player* m_owner
    void add_point_to_owner(int point);

    // when it end of exproded, make child corresponding direction, given by 'm_blast_life_time'
    std::vector<std::shared_ptr<CObj>> update()override;


    //collisions
    void collision(CObj& other) override;
    void collision( ExtraEnemy& ) override;
    void collision( Wall&) override;
    void collision( Player&) override;
    void collision( BreakableWall&) override;
    void collision( Bomb&) override;
    void collision( Blast&) override;
    void collision( Item&) override;
};
