#include "CObj.h"

CObj::CObj(const SDL_Rect& pos):m_texture(nullptr),m_position(pos),m_alive(true){}
    
void CObj::draw(SDL_Renderer* renderer, int step){
    if(!m_alive) return ;
    SDL_Rect accual_rect = m_position;
    accual_rect.x *= step;
    accual_rect.y *= step;
    accual_rect.h *= step;
    accual_rect.w *= step;
    SDL_RenderCopy(renderer, m_texture, nullptr, &accual_rect);
}
bool CObj::alive(){return m_alive;}

bool CObj::detect_collision(const CObj& other){
    return m_position.x == other.m_position.x && m_position.y == other.m_position.y;
}


void CObj::setRectTex(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b){
    SDL_Surface* rectSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(rectSurface, NULL, SDL_MapRGB(rectSurface->format, r, g, b));
    m_texture = SDL_CreateTextureFromSurface(renderer, rectSurface);
    SDL_FreeSurface(rectSurface);

    if(!m_texture) throw std::runtime_error("load tex");
}

// Set the Image Texture to the object from given path image
void CObj::setImgTex(SDL_Renderer* renderer, const std::string& texfile){
    SDL_Surface* surf= IMG_Load(texfile.c_str());
    m_texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if(!m_texture) throw std::runtime_error("load tex");
}