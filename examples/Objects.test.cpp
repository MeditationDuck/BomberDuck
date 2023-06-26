#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <algorithm>
#include <random>
#include <memory>


#include <assert.h>

#include "../src/CObj.h"
#include "../src/CMovable.h"
#include "../src/ExtraEnemy.h"
#include "../src/Wall.h"
#include "../src/BreakableWall.h"
#include "../src/Player.h"
#include "../src/Bomb.h"
#include "../src/Blast.h"
#include "../src/Item.h"
#include "../src/dir.h"
#include "../src/Mode.h"



using namespace std;

int main(){

    Player* player_pointer = new Player(SDL_Rect{0,0,0,0},1, 1.0);
    CObj* CObj_pointer_player = player_pointer;
    CObj* CObj_pointer_wall = new Wall(SDL_Rect{0,0,0,0});
    // location is same thus should detect that it is collide.
    assert(CObj_pointer_player->detect_collision(*CObj_pointer_wall) == true);

    
    player_pointer->NextMove(dir::left);// in the game player move using Player pointer.
    CObj_pointer_player->update(); //update then apply next location set by next Move.
    assert(CObj_pointer_player->detect_collision(*CObj_pointer_wall) == false);
    player_pointer->NextMove(dir::right); // called by keyboard

    CObj_pointer_player->update(); 
    //once left once right thus those shoud same position.
    assert(CObj_pointer_player->detect_collision(*CObj_pointer_wall) == true);

    //call collision effect. by double dispatch.
    CObj_pointer_player->collision(*CObj_pointer_wall); //This calls collision(Player&) in wall class. Wall is nothing to do.   
    CObj_pointer_wall->collision(*CObj_pointer_player); //This called collision(Wall&) in Player class. Player back one posision.
    
    //this should false. Since by collision effect, player move back because player can not overrup wall.
    assert(CObj_pointer_player->detect_collision(*CObj_pointer_wall) == false);

    Player* player_pointer_two =  new Player(SDL_Rect{0,0,0,0},1, 1.0);

    CObj* CObj_pointer_blast = new Blast(SDL_Rect{0,0,0,0}, player_pointer_two, dir::none, 1.0);


    // this calls collision(CObj&) in blast class.
    // in this method calls collision(Blast&) in Player class.
    // then player change alive status.
    CObj_pointer_blast->collision(*CObj_pointer_player);

    //when collide with blast player will die.
    assert(CObj_pointer_player->alive() == false);

    delete CObj_pointer_player;
    delete CObj_pointer_wall;
    delete CObj_pointer_blast;
    delete player_pointer_two;

    //destructor test
    shared_ptr<Player>CObj_shared_player(new Player(SDL_Rect{0,0,0,0},1, 1.0));

    shared_ptr<CObj>CObj_shared_pointer_player = CObj_shared_player;

    shared_ptr<CObj>CObj_shared_pointer_bomb(new Bomb(SDL_Rect{1,1,0,0}, CObj_shared_player.get(), false, false, 0.0));
    //After 0 second bomb make blasts.
    vector<shared_ptr<CObj>>child_blasts = CObj_shared_pointer_bomb->update();
    assert(child_blasts.size() == 5);
    assert(CObj_shared_pointer_bomb->alive() == false);

    
    //child blast of life time is (bomb life time)/5 thus this time even 0.
    assert(child_blasts[0]->alive() == true);
    vector<shared_ptr<CObj>>child_blasts_child = child_blasts[0]->update();
    // this bomb does not had unlimited blast thus no child. this setted false at third argument of Bomb. 
    assert(child_blasts_child.size() == 0);
    // update method checks the time of life time.
    assert(child_blasts[0]->alive() == false);

    


}