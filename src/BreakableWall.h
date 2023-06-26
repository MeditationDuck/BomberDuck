
#pragma once
#include <iostream>
#include <vector>

#include "CObj.h"


/**
 * @brief Brealable Wall class. 
 *     This wall can be break by blast.
 * 
 */
class BreakableWall: public CObj{
public:

    BreakableWall(const SDL_Rect& pos):CObj(pos){}

    void init(SDL_Renderer* renderer) override {
        CObj::setRectTex(renderer, 200,200,200);
    }

    std::vector<std::shared_ptr<CObj>> update()override{ return {};}

    //collisions
    void collision(CObj& other) override {other.collision(*this);}
    void collision( ExtraEnemy& ) override{}
    void collision( Wall& ) override{}
    void collision( Player& ) override{}
    void collision( BreakableWall& ) override{}
    void collision( Bomb& ) override{}
    // blast breaks the breakablewall.
    void collision( Blast& ) override{m_alive = false;}
    void collision(Item&) override{}
};
