#include "ExtraEnemy.h"

void ExtraEnemy::move_extra(){
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - m_pitch;
    if (elapsed_seconds.count() < m_move_cycle) return;
   
    NextMove(static_cast<dir>(rand()%4)); // 4 to aboid none since We want extra enemy to try to move every pitch.
    m_pitch = std::chrono::steady_clock::now();
}


ExtraEnemy::ExtraEnemy(SDL_Rect pos, double move_cycle)
        : CMovable(pos), m_move_cycle(move_cycle),m_pitch(std::chrono::steady_clock::now()){}

void ExtraEnemy::init(SDL_Renderer* renderer) {
    setImgTex(renderer, "assets/ExtraEnemy.bmp");
}

std::vector<std::shared_ptr<CObj>> ExtraEnemy::update(){
    CMovable::update();
    move_extra();
    return {};
}

void ExtraEnemy::collision(CObj& other) {other.collision(*this);}
void ExtraEnemy::collision( ExtraEnemy&) {
    back();
}
void ExtraEnemy::collision( Wall& ) {
    back();
}
void ExtraEnemy::collision( Player& ) {
}
void ExtraEnemy::collision( BreakableWall& ) {
    back();
}
void ExtraEnemy::collision( Bomb& ) {
    back();
}
void ExtraEnemy::collision( Blast&) {
    m_alive = false;
}
void ExtraEnemy::collision( Item&) {}