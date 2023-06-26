#include "Player.h"

#include "Bomb.h"

// first bomb without time restriction thus set 10 second before.
Player::Player(SDL_Rect pos,int texture_type, double bomb_cycle)
        : CMovable(pos),m_bomb_cycle(bomb_cycle), m_texture_type(texture_type), m_points(0), m_tmp_points(0), m_drop_bomb(false),m_exprode_bomb(false), m_items{false, false, false}, 
        m_bombdrop_cycle_start(std::chrono::steady_clock::now()  - std::chrono::seconds(10)) {}

void Player::init(SDL_Renderer* renderer){
    switch(m_texture_type){
        case 1:
            setImgTex(renderer, "assets/Player2.bmp");
            break;
        case 0:
        default:
            setImgTex(renderer, "assets/Player.bmp");
    }
}
void Player::addPoint(int p){ m_tmp_points += p;}

int Player::getPoint(){return m_points;}

void Player::drop_bomb(){
    auto current_time = std::chrono::steady_clock::now();
    if(m_items[static_cast<int>(PowerUpType::unlimited_bomb)] == false){
        std::chrono::duration<double>elapsed_seconds = current_time - m_bombdrop_cycle_start;
        if (elapsed_seconds.count() < m_bomb_cycle)return;
    }
    m_bombdrop_cycle_start = current_time;
    
    if(m_items[static_cast<int>(PowerUpType::remote_control)] == true){
        if(m_exprode_bomb == false){
            m_exprode_bomb = true;
        }else{
            m_drop_bomb = true;
            m_exprode_bomb = false;
        }
    }else{
        m_drop_bomb = true;
        m_exprode_bomb = false;
    }
    
}

 bool Player::drop_exprode() const {
    return m_exprode_bomb;
 }

std::vector<std::shared_ptr<CObj>>  Player::update(){
    CMovable::update();
    m_points = m_tmp_points;

    // for()

    if(!m_drop_bomb) return {};
    std::vector<std::shared_ptr<CObj>> ret;
    m_items = m_tmp_items; 
    bool unlimited_blast = m_items[static_cast<int>(PowerUpType::unlimited_blast)];
    bool remote_control = m_items[static_cast<int>(PowerUpType::remote_control)];
    ret.emplace_back(new Bomb(m_position, this, unlimited_blast, remote_control, m_bomb_cycle));
    m_drop_bomb = false;
    return ret;
}

void Player::collision(CObj& other) {other.collision(*this);}
void Player::collision( ExtraEnemy&) {
    m_alive = false;
}
void Player::collision( Wall&) {
    back();
}
void Player::collision( Player&) {
    back();
}
void Player::collision( BreakableWall&) {
    back();
    m_tmp_items = m_items;
    m_tmp_points = m_points;
}
void Player::collision( Bomb&) {
    back();
}
void Player::collision( Blast&) {
    m_alive = false;
}
void Player::collision(Item& other){
    
    addPoint(10);
    m_tmp_items[static_cast<int>(other.getItemType())] = true;
}
