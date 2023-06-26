#include "CMovable.h"


CMovable::CMovable(SDL_Rect pos)
    : CObj(pos),m_next(dir::none), m_prev_pos(pos){}

void CMovable::NextMove(const dir& direction){
    m_next = direction;
}

std::vector<std::shared_ptr<CObj>> CMovable::update() {
    m_prev_pos = m_position;
    switch(m_next){
        case dir::down:
            m_position.y += 1;
            break;
        case dir::left:
            m_position.x -= 1;
            break;
        case dir::up:
            m_position.y -= 1;
            break;
        case dir::right:
            m_position.x += 1;
            break;
        case dir::none:
        default:
            break;
    }
    m_next = dir::none;
    return {};
}

void CMovable::back(){
    m_position = m_prev_pos;
}