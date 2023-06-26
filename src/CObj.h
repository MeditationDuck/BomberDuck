#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>

class CMovable;
class ExtraEnemy;
class Wall;
class Blast;
class Player;
class BreakableWall;
class Bomb;
class Item;

/**
 * @brief Base class of all game objects
 */
class CObj{ 
    SDL_Texture* m_texture;
protected:
    SDL_Rect m_position; // store object rectangle information.
    // In this game we only use multiple of 'step' of location
    // and object width and height is 'step' by default.
    bool m_alive; // check every update in game board class
                // if it set false then it will destroyed.
public:
    
    CObj(const SDL_Rect& pos);
    
    virtual ~CObj() = default;

    //basically setting texture
    virtual void init(SDL_Renderer* renderer) = 0;

    // update each object status, some object have trigar to create new objects.
    virtual std::vector<std::shared_ptr<CObj>> update() = 0;

    /**
     * @brief draw object
     * 
     * @param renderer 
     * @param step side length of gameboard cell
     */
    void draw(SDL_Renderer* renderer, int step);

    bool alive();

    //compare m_position
    bool detect_collision(const CObj& other);

    //collision for double dispatch
    virtual void collision(CObj& other) = 0;
    //collision for each objecs
    virtual void collision( ExtraEnemy&) = 0;
    virtual void collision( Wall&) = 0;
    virtual void collision( Player&) = 0;
    virtual void collision( BreakableWall&) = 0;
    virtual void collision( Bomb& ) = 0;
    virtual void collision( Blast& other) = 0;
    virtual void collision( Item&) = 0;
    
    //setting texture functions

    // Set the Rectanble One color Texture to the object
    void setRectTex(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b);
    // Set the Image Texture to the object from given path image
    void setImgTex(SDL_Renderer* renderer, const std::string& texfile);
};