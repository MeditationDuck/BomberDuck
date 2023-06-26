#include "Item.h"

Item::Item(SDL_Rect pos, const PowerUpType& type):CObj(pos), m_uptype(type){}

void Item::init(SDL_Renderer* renderer) {
    switch(m_uptype){
        case PowerUpType::unlimited_blast:
            setImgTex(renderer, "assets/UnlimitedBlast.bmp");
            break;
        case PowerUpType::unlimited_bomb:
            setImgTex(renderer, "assets/UnlimitedBomb.bmp");
            break;
        case PowerUpType::remote_control:
            setImgTex(renderer, "assets/switch.bmp");
            break;
        default:
            throw std::runtime_error("Item object does not have Power Up type");
    }
}

PowerUpType Item::getItemType() const {
    return m_uptype;
}
/**
 * @brief update previous position.
 * 
 */
std::vector<std::shared_ptr<CObj>> Item::update() {
    return {};
}

void Item::collision(CObj& other) {other.collision(*this);}
void Item::collision( ExtraEnemy&) {}
void Item::collision( Wall&) {}
void Item::collision( Player&) {
    m_alive = false;
}
void Item::collision( BreakableWall&) {}
void Item::collision( Bomb&) {}
void Item::collision( Blast&) {
}
void Item::collision(Item&){}
