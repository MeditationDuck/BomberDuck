#include "GameBoard.h"


void GameBoard::render_init(SDL_Renderer* renderer){
    for(auto& c:m_objs) c->init(renderer);
}


void GameBoard::remove_neighbor(const std::pair<int,int>& target){

    m_free.erase(remove_if(m_free.begin(), m_free.end(),
                        [target](const std::pair<int, int>& p) {
                            return (abs(target.first - p.first) <= 2) && (abs(target.second - p.second) <= 2);
                        }),
        m_free.end());
}

void GameBoard::addEnemies(int num, const double& enemy_move_cycle){
    for(int i = 0; i < num; i++){
        std::uniform_int_distribution<int> distribution(0, m_free.size()-1);
        int randint = distribution(m_random_generator);
        std::pair<int, int> tmp = m_free[randint];
        addExtraEnemy(tmp.first, tmp.second, enemy_move_cycle);
        remove(m_free.begin(), m_free.end(), tmp);
    }
}

void GameBoard::addPlayer(int x, int y, int texture_type) {
    SDL_Rect pos = {x, y, 1, 1};
    // this case player can put one bomb in 2.0 second. and bomb exprode in 2 second after put by player.
    std::shared_ptr<Player> player(new Player(pos,texture_type,2.0));
    if(keyboard_player1 == nullptr){
    keyboard_player1 = player;
    }else if(keyboard_player2 == nullptr){
        keyboard_player2 = player;
    }else{
        // accept only two player in game board
        return;
    }
    m_objs.emplace_back(player);
}

void GameBoard::addWall(int x, int y){
    SDL_Rect pos = {x, y, 1, 1};
    m_objs.emplace_back(new Wall(pos));
}
void GameBoard::addItem(int x, int y){
    SDL_Rect pos = {x, y, 1, 1};
    std::uniform_int_distribution<int>distribution(0,2);

    switch(distribution(m_random_generator)){
        case 0:
            m_objs.emplace_back(new Item(pos, PowerUpType::unlimited_blast));
            break;
        case 1:
            m_objs.emplace_back(new Item(pos, PowerUpType::unlimited_bomb));
            break;
        case 2:
            m_objs.emplace_back(new Item(pos, PowerUpType::remote_control));
            break;
        default:
            throw std::logic_error("not match power up item selection when adding Item.");
    }
}

void GameBoard::addBreakableWall(int x, int y){
    // put item before breakable wall to render under the breakable wall.
    double item_rate = 2;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    if( distribution(m_random_generator) <= item_rate){
        addItem(x,y);
    }
    SDL_Rect pos = {x, y, 1, 1};
    m_objs.emplace_back( new BreakableWall(pos));
}
void GameBoard::addExtraEnemy(int x, int y, const double& move_cycle){
    SDL_Rect pos = {x, y, 1, 1};
    m_objs.emplace_back( new ExtraEnemy(pos, move_cycle));
}

void GameBoard::clear_objs(){
    m_objs.erase(remove_if(m_objs.begin(), m_objs.end(), [](const std::shared_ptr<CObj>& a){return !a->alive(); }), m_objs.end());
}


GameBoard::GameBoard(int step, const Mode& mode):m_step(step), keyboard_player1(nullptr), keyboard_player2(nullptr), m_playmode(mode), m_board_width(0), m_board_height(0), m_random_generator(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count())){
} 

int GameBoard::step() const {
    return m_step;
}
void GameBoard::Setplaymode(const Mode& playmode){
    m_playmode = playmode;
}

Mode GameBoard::playmode() const {
    return m_playmode;
}
void GameBoard::clean(){
    keyboard_player1.reset();
    keyboard_player2.reset();
    m_objs.clear();
}

std::pair<int, int> GameBoard::LoadMap(const std::string& map_path){
    std::string line;
    std::fstream mapFile;
    mapFile.open(map_path);
    if(!mapFile){
        throw std::runtime_error("can not open mapfile");
    }
    int y = 0;
    m_board_height = 0;
    m_board_width = 0;
    std::vector<std::pair<int, int>> player_location;
    while(getline(mapFile, line)){
        if(m_board_height == 0){ m_board_width = line.length();}
        
        if(line.size() != (size_t)m_board_width){
            throw std::runtime_error("Invalid x length of file");
        }
        for(int x = 0 ; x < m_board_width; x++){  
            switch(line[x]){
                case '1':
                    addWall(x, y);
                    break;
                case '2':
                    addBreakableWall(x, y);
                    break;
                case 'p':
                    addPlayer(x, y, 0);
                    player_location.push_back({x,y});
                    break;
                case 'P':
                    if(playmode() == Mode::MultiPlayer){
                        addPlayer(x, y, 1);
                        player_location.push_back({x,y});
                    }
                    break;
                default:
                    m_free.push_back({x, y});
                    break;
            }
        }
        y++;
    }
    if(mapFile.bad()){
        throw std::runtime_error("map file error");
    }else if(!mapFile.eof()){
        throw std::runtime_error("map file format error");
    }
    mapFile.close();

    for(auto& e:player_location){
        remove_neighbor(e);
    }
    
    m_board_height = y;
    return {m_board_width, m_board_height};
}

std::pair<int, int> GameBoard::getPlayerPoints() const {
    std::pair<int, int> points = {0,0};
    points.first = keyboard_player1->getPoint();

    if(m_playmode == Mode::MultiPlayer){
        points.second = keyboard_player2->getPoint();
    }
    return points;
}

void GameBoard::init(SDL_Renderer* renderer){
    // put enemy randomly, this means there is one enemy in 16 game cell of rate.
    // and the enemy can move once in 0.2 seconds.
    addEnemies(m_board_height*m_board_width/32, 1.0);
    render_init(renderer);
}


bool GameBoard::update(SDL_Renderer* renderer){

    // update objects
    for(auto& c: m_objs){
        std::vector<std::shared_ptr<CObj>> ret = c->update();
        if(ret.size() != 0){
            for(auto e:ret){
                m_objs.emplace_back(e);
                m_objs.back()->init(renderer);
            }
        }
    }
    
    // corrision detection.
    for(auto& p : m_objs){
        for(auto& o: m_objs){
            if(p == o) continue;
            if(p->detect_collision(*o)){
                p->collision(*o);
            }
        }
    }
    
    if(m_playmode == Mode::SinglePlayer){
        if(!keyboard_player1->alive()){
            //gameover
            return false;
        }
    }else{ 
        if(!keyboard_player1->alive() && !keyboard_player2->alive()){
            //gameover
            return false;
        }
    }

    clear_objs();
    return true;
}




void GameBoard::render(SDL_Renderer* renderer){
    for(auto& c: m_objs) c->draw(renderer, m_step);
}

void GameBoard::player_control_by_keyboard(const Uint8* state){
    if(keyboard_player1 != nullptr){
        if (state[SDL_SCANCODE_W]) {
            keyboard_player1->NextMove(dir::up);
        }else
        if (state[SDL_SCANCODE_A]) {
            keyboard_player1->NextMove(dir::left);
        }else
        if (state[SDL_SCANCODE_S]) {
            keyboard_player1->NextMove(dir::down);
        }else
        if (state[SDL_SCANCODE_D]) {
            keyboard_player1->NextMove(dir::right);
        }else
        if (state[SDL_SCANCODE_C]) {
            keyboard_player1->drop_bomb();
        }
    }

    if (keyboard_player2 != nullptr) {
        if (state[SDL_SCANCODE_UP]) {
            keyboard_player2->NextMove(dir::up);
        }else if (state[SDL_SCANCODE_LEFT]) {
            keyboard_player2->NextMove(dir::left);
        }else if (state[SDL_SCANCODE_DOWN]) {
            keyboard_player2->NextMove(dir::down);
        }else if (state[SDL_SCANCODE_RIGHT]) {
            keyboard_player2->NextMove(dir::right);
        }
        if (state[SDL_SCANCODE_SLASH]) {
            keyboard_player2->drop_bomb();
        }
    }
}


