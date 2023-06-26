#include "Wall.h"


Wall::Wall(const SDL_Rect& pos):CObj(pos){}

void Wall::init(SDL_Renderer* renderer) {
    CObj::setRectTex(renderer, 100,100,100);
}
std::vector<std::shared_ptr<CObj>> Wall::update(){ return {};}

void Wall::collision(CObj& other) {other.collision(*this);}
void Wall::collision( ExtraEnemy&) {}
void Wall::collision( Wall&) {}
void Wall::collision( Player&) {}
void Wall::collision( BreakableWall&) {}
void Wall::collision( Bomb&) {}
void Wall::collision( Blast&) {}
void Wall::collision( Item&) {}