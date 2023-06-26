#include <assert.h>

#include "../src/GameBoard.h"

using namespace std;

int main(){
    GameBoard gb;
    bool test = false;
    try{
        gb.LoadMap("nosuchfile");
    }catch (const std::runtime_error&) {
        // must throw runtime_error
        test = true;
    } 
    assert(test);

    test = false;
    try{
        gb.LoadMap("tests/invalid_map.map");
    }catch (const std::runtime_error&) {
        // must throw runtime_error
        test = true;
    } 
    assert(test);

    test = false;
    try{
        // this is valid map file
        gb.LoadMap("assets/map3.map");
    }catch(...){

        test= true;
    }
    // it should witout exception.
    assert(!test);

    // also, Gameboard loads game objects thus check memory leak in deconstructor.
    // in test files we use -fsanitize=address
}