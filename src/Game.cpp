#include "Game.h"

#include "Mode.h"


// function to draw text.
void Game::drawText(const std::string& text, SDL_Color color, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_Rect rect = {x - surface->w / 2, y - surface->h / 2, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Init SDL and create window on screen.
void Game::init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    m_window = SDL_CreateWindow("BomberMan(Duck)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
    if(!m_window) throw std::runtime_error("creating window");

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    m_renderer = SDL_CreateRenderer(m_window, -1, render_flags);
    if(!m_renderer) throw std::runtime_error("loading font");

    m_font = TTF_OpenFont("assets/visitor1.ttf", 24);
    if(!m_font) throw std::runtime_error("loading font");

    SDL_SetRenderDrawColor(m_renderer, 20,20,20,255);
}

// User select game mode and load maps.
bool Game::ChooseGameMode(){
    m_screenWidth = 320;
    m_screenHeight = 140;
    init();
    Mode mode = Mode::SinglePlayer;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&m_event)) {
            if (m_event.type == SDL_QUIT) {
                running = false;
                clean();
                return false;
            } else if (m_event.type == SDL_KEYDOWN) {
                switch (m_event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_DOWN:
                        mode = (mode == Mode::SinglePlayer) ? Mode::MultiPlayer : Mode::SinglePlayer;
                        break;
                    case SDLK_RETURN:
                        running = false;
                        break;
                }
            }
        }
        SDL_RenderClear(m_renderer);
        drawText("Single Player", (mode == Mode::SinglePlayer) ? red : white, m_screenWidth/2, m_screenHeight/6*2);
        drawText("Multi Player", (mode == Mode::MultiPlayer) ? red : white, m_screenWidth/2, m_screenHeight/6*3);
        SDL_RenderPresent(m_renderer);
    }
    m_gameboard.Setplaymode(mode);
    clean();
    return true;
}

void Game::LoadMap(){
    try{
        std::pair<int, int> board_size = m_gameboard.LoadMap("assets/map2.map");
        m_screenWidth = board_size.first * m_gameboard.step();
        m_screenHeight = board_size.second * m_gameboard.step();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("loading map");
    }
}

bool Game::ConcludeGame(){
    bool running = true;
    bool retry = true;
    //register game result to record and get top records.
    std::pair<int, int> points = m_gameboard.getPlayerPoints();
    m_gamerecods.ResultInsert({points.first});
    if(m_gameboard.playmode() == Mode::MultiPlayer){
        m_gamerecods.ResultInsert({points.second});
    }
    Record rec = m_gamerecods.GetRecord();

    while(running){
        // user input 
        while(SDL_PollEvent(&m_event)){
            if(m_event.type == SDL_QUIT){
                running = false;
                return false;
            }else if(m_event.type == SDL_KEYDOWN){
                switch(m_event.key.keysym.sym){
                    case SDLK_UP:
                    case SDLK_DOWN:
                        retry = !retry;
                        break;
                    case SDLK_RETURN:
                        running = false;
                        break;
                }
            }
        }
        //draw texts to screen

        SDL_RenderClear(m_renderer);
        //Draw result of this game.
        if(m_gameboard.playmode() == Mode::SinglePlayer){
            drawText("Result: " + std::to_string(points.first) + " pt.", red ,  m_screenWidth/3, m_screenHeight/6);
        }else{
            drawText("Result: Player1 " + std::to_string(points.first) + " pt.  Player2 "+ std::to_string(points.second) + " pt.", red ,  m_screenWidth/3, m_screenHeight/6);
        }
        // Draw top five records from file and Player results.
        drawText("Records", red, m_screenWidth/2,m_screenHeight/8*3);
        drawText("RANK         POINT     ", white, m_screenWidth/2, m_screenHeight/2+m_screenHeight/16*-1);
        for(int i = 0; i < rec.count; i++){
            drawText(std::to_string(i+1) + ".         " + std::to_string(rec.TopRecords[i].point) + " pt", white, m_screenWidth/2, m_screenHeight/2+m_screenHeight/16*i);
        }

        drawText("Retry", (retry? red:white), m_screenWidth/2, m_screenHeight/6*5);
        drawText("Quit", (!retry? red:white), m_screenWidth/2, m_screenHeight/12*11);
        SDL_RenderPresent(m_renderer);
    }
    //write record to file.
    m_gamerecods.writeToFile();
    return retry;
}

void Game::render(){
    SDL_RenderClear(m_renderer);
    //render game objects
    m_gameboard.render(m_renderer);
    
    // draw texts, e.g. player points
    std::pair<int, int> points = m_gameboard.getPlayerPoints();
    drawText("player 1 points: " + std::to_string(points.first), red,  m_screenWidth/4, m_gameboard.step()/3);
    if(m_gameboard.playmode() == Mode::MultiPlayer){
       drawText("player 2points: " + std::to_string(points.second), red,  m_screenWidth/4, m_gameboard.step()/3 * 2);
    }
    if(m_gamerecods.GetRecord().count != 0){
        drawText("Records: " + std::to_string(m_gamerecods.GetRecord().TopRecords[0].point), red, m_screenWidth/6*5, m_gameboard.step()/3);
    }
    SDL_RenderPresent(m_renderer);
}

// handle user input events.
void Game::HandleEvent(){
    SDL_PollEvent(&m_event);
    if (m_event.type == SDL_KEYDOWN) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        m_gameboard.player_control_by_keyboard(state);
    }
    switch(m_event.type){
        case SDL_QUIT:
            m_state = EXIT;
            break;
        default:
            break;
    }
}
// clean SDLs.
void Game::clean(){
    TTF_CloseFont(m_font);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

Game::Game()
    : m_window(nullptr), m_renderer(nullptr), m_font(nullptr), m_event(), m_screenWidth(0), m_screenHeight(0), m_gameboard(){}

void Game::run(){
    bool play = true;
    while(play){
        m_state = false;
        // Choosing play mode
        if(!ChooseGameMode()){return;}
        LoadMap();            

        init();
        m_gamerecods.LoadRecords("BestScore.rec");
        m_gameboard.init(m_renderer);
        //PLAYING GAME LOOP
        while(m_state != EXIT ){
            render();
            HandleEvent();

            if(!m_gameboard.update(m_renderer)){
                m_state = EXIT;
            }
        }
        if(!ConcludeGame()) play = false;
        m_gameboard.clean();
        clean();
    }
}