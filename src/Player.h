#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <chrono>

#include "CMovable.h"
#include "Item.h"

/**
 * @brief Child class of CMovable 
 * 
 */
class Player: public CMovable {
     
    const double m_bomb_cycle; // Player can set bomb once m_bomb_cycle by default and bomb exprode after this second.
    int m_texture_type;
    int m_points; // current points
    int m_tmp_points;// tmp point for item corrision.
    bool m_drop_bomb; // drop bomb when update was called
    bool m_exprode_bomb; // exprode bomb for remote control item.
    std::array<bool,3> m_items; // Power Items two is the number of item.
    std::array<bool,3> m_tmp_items; // Power Items two is the number of item.
    std::chrono::steady_clock::time_point m_bombdrop_cycle_start;
public:

    Player(SDL_Rect pos, int texture_type, double bomb_cycle = 2.0);
    
    //set texture
    void init(SDL_Renderer* renderer) override;

    void addPoint(int p);

    //return player points
    int getPoint();

    // make m_drop_bomb to true if satisfy the condition
    void drop_bomb();

    // controll sign for remote control item.
    bool drop_exprode() const;
    
    //update player, when m_drop_bomb true return bomb object.
    std::vector<std::shared_ptr<CObj>>  update() override;

    //collision for double dispatch.
    void collision(CObj& other) override;

    void collision( ExtraEnemy&) override;
    void collision( Wall&) override;
    void collision( Player&) override;
    void collision( BreakableWall&) override;
    void collision( Bomb&) override;
    void collision( Blast&) override;
    // change item status, i.e. modify m_items
    void collision( Item&) override;
};
