#include "Blast.h"

#include "Player.h"

Blast::Blast(const SDL_Rect& pos, Player* owner, dir blast_direction, double blast_life_time)
        : CObj(pos), m_blast_life_time(blast_life_time), m_life_start_time(std::chrono::steady_clock::now()), m_owner(owner), m_blast_direction(blast_direction){}

void Blast::init ( SDL_Renderer* renderer){
    CObj::setRectTex(renderer, 255,0,0);
}

void Blast::add_point_to_owner(int point){
    m_owner->addPoint(point);
}

std::vector<std::shared_ptr<CObj>> Blast::update() {
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - m_life_start_time;
    if (elapsed_seconds.count() < m_blast_life_time)return {};
    m_alive = false;
    std::vector<std::shared_ptr<CObj>> childs;
    switch(m_blast_direction){
        case dir::left:{
            SDL_Rect left = m_position;
            left.x -=1;
            childs.emplace_back(new Blast(left, m_owner, dir::left, m_blast_life_time));
            break;
        }
        case dir::right:{
            SDL_Rect right = m_position;
            right.x +=1;
            childs.emplace_back(new Blast(right, m_owner, dir::right, m_blast_life_time));
            break;
        }    
        case dir::up:{
            SDL_Rect up = m_position;
            up.y -=1;
            childs.emplace_back(new Blast(up, m_owner, dir::up, m_blast_life_time));
            break;
        }
        case dir::down:{
            SDL_Rect down = m_position;
            down.y +=1;
            childs.emplace_back(new Blast(down, m_owner, dir::down, m_blast_life_time));
            break;
        }
        default:
            break;
    }
    return childs;
}

void Blast::collision(CObj& other)  {other.collision(*this);}
void Blast::collision( ExtraEnemy& )  {
    add_point_to_owner(10);
    m_alive = false;
}

//brast can not overrup on the wall
void Blast::collision( Wall&)  {
    m_alive = false;
}

void Blast::collision( Player& other)  {
    if(m_owner == &other) return;// if owner died caused by themself, do not give point.
    add_point_to_owner(50);
    m_alive = false;
}

//brast can not overrup on the bleakablewall
void Blast::collision( BreakableWall&)  {
    add_point_to_owner(5);
    m_alive = false;
}
void Blast::collision( Bomb&)  {
     m_alive = false;
}
void Blast::collision( Blast&)  {
    m_alive = false;
}
void Blast::collision( Item&)  {
    m_alive = false;
}