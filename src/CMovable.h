#pragma once
#include <iostream>
#include <vector>

#include "CObj.h"
#include "dir.h"


/**
 * @brief Child class of CObj class and Base class of Movable objects.
 * 
 */
class CMovable: public CObj {

    dir m_next;// direction for next move
    SDL_Rect m_prev_pos; // previous location
public:
    CMovable(SDL_Rect pos);

    // set next move to m_next as direction.
    void NextMove(const dir& direction);

    /**
     * @brief update previous position.
     *  and move to new position set by NextMove()
     */
    std::vector<std::shared_ptr<CObj>> update() override;

    /**
     * @brief move back to previous position.
     * 
     */
    void back();
};

