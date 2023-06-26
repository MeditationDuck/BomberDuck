
#pragma once
#include <iostream>
#include <vector>

#include "CObj.h"


/**
 * @brief Wall class. object of this never broken.
 * 
 */
class Wall: public CObj {

public:
    Wall(const SDL_Rect& pos);

    void init(SDL_Renderer* renderer) override;
    std::vector<std::shared_ptr<CObj>> update()override;
   
    void collision(CObj& other) override;
    void collision( ExtraEnemy&) override;
    void collision( Wall&) override;
    void collision( Player&) override;
    void collision( BreakableWall&) override;
    void collision( Bomb&) override;
    void collision( Blast&) override;
    void collision( Item&) override;
};