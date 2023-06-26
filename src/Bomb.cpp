# include "Bomb.h"

#include "Player.h"
#include "Blast.h"
#include "CObj.h" 


Bomb::Bomb(const SDL_Rect& pos, Player* owner, bool unlimited_blast, bool remote_control, double time_to_bomb)
        : CObj(pos),m_time_to_bomb(time_to_bomb), m_life_start_time(std::chrono::steady_clock::now()), m_owner(owner), m_unlimited_blast(unlimited_blast), m_remote_controlled(remote_control){}

void Bomb::init ([[maybe_unused]] SDL_Renderer* renderer){
    CObj::setRectTex(renderer, 0,0,255);
}

std::vector<std::shared_ptr<CObj>> Bomb::update(){

    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - m_life_start_time;
    // if its not controled by remote. exprode trigger is time
    if (elapsed_seconds.count() < m_time_to_bomb && !m_remote_controlled){
        return {};
    }
    // if its controlled, exprode by m_owner exprode sign.
    if(m_remote_controlled && !m_owner->drop_exprode()){
        return {};
    }

    m_alive = false;
    int range = 1;

    std::vector<std::shared_ptr<CObj>> childs;
    double childs_blast_life_time = m_time_to_bomb/5;

    if(m_unlimited_blast == false){
        for(int j = -range; j <= range; j++){
            if(j == 0){
                for(int i = -range; i <= range; i++){
                    SDL_Rect pos = m_position;
                    pos.x +=(j);
                    pos.y +=(i);
                    childs.emplace_back(new Blast(pos, m_owner, dir::none, childs_blast_life_time));
                }
            }else{
                SDL_Rect pos = m_position;
                pos.x +=(j);
                childs.emplace_back(new Blast(pos, m_owner, dir::none, childs_blast_life_time));
            }
            
        }
        
    }else{
        SDL_Rect left = m_position;
        left.x -=1;
        SDL_Rect right = m_position;
        right.x +=1;
        SDL_Rect up = m_position;
        up.y -=1;
        SDL_Rect down = m_position;
        down.y +=1;
        childs.emplace_back(new Blast(left, m_owner, dir::left, childs_blast_life_time));
        childs.emplace_back(new Blast(right, m_owner, dir::right, childs_blast_life_time));
        childs.emplace_back(new Blast(up, m_owner, dir::up, childs_blast_life_time));
        childs.emplace_back(new Blast(down, m_owner, dir::down, childs_blast_life_time));
        childs.emplace_back(new Blast(m_position, m_owner, dir::none, childs_blast_life_time));
    }
    return childs;
}
