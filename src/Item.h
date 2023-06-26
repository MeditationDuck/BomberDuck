#pragma once
#include <iostream>
#include <vector>

#include "CObj.h"
#include "dir.h"

enum class PowerUpType{
    unlimited_blast,
    unlimited_bomb,
    remote_control,
};


/**
 * @brief Child class of CObj class and Base class of Movable objects.
 * 
 */
class Item: public CObj {
    PowerUpType m_uptype;
public:
    Item(SDL_Rect pos, const PowerUpType& type);

    void init(SDL_Renderer* renderer) override;

    /**
     * @brief update previous position.
     * 
     */
    std::vector<std::shared_ptr<CObj>> update() override;
    
    PowerUpType getItemType() const ;

    void collision(CObj& other) override;
    void collision( ExtraEnemy&) override;
    void collision( Wall&) override;
    void collision( Player&) override;
    void collision( BreakableWall&) override;
    void collision( Bomb&) override;
    void collision( Blast&) override;
    void collision( Item&) override;

    /**
     * @brief restore previous position.
     * 
     */
    void back();
};
