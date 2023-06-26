#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// Game result to sotre file and evaluate the rank.
struct GameResult{
    int point = 0;
    bool operator < (const GameResult& other) const {
        return this->point < other.point;
    }
     bool operator > (const GameResult& other) const {
        return this->point > other.point;
    }
};
//Format that stored in BestScore.rec
struct Record{
    int count = 0;
    std::vector<GameResult> TopRecords;
};

//Manage Game Records
class GameRecords{
  
    static constexpr size_t m_store_count = 5;
    std::string m_record_path;
    Record m_gamerecords;

    //Read records from file
    Record readGameRecords(){
        Record record;
        std::ifstream ifs(m_record_path, std::ios::binary);
        if(!ifs.is_open()){
            // file not found.
            return record;
        }

        ifs.read(reinterpret_cast<char*>(&record.count), sizeof(record.count));

        record.TopRecords.resize((size_t)record.count);

        for(int i = 0; i < record.count; i++){
            ifs.read(reinterpret_cast<char*>(&record.TopRecords[i]), sizeof(record.TopRecords[0]));
        }
        if(!ifs){
            ifs.close();
            throw std::runtime_error("Reading record file");
        }
        ifs.close();
        return record;
    }


public:
    //Read records to file
    void writeToFile() {
        std::ofstream ofs(m_record_path, std::ios::binary);

        if(!ofs){
            ofs.close();
            std::remove(m_record_path.c_str());
            throw std::runtime_error("writing file");
        }

        ofs.write(reinterpret_cast<const char*>(&m_gamerecords.count), sizeof(m_gamerecords.count));
        if(!ofs){
            ofs.close();
            std::remove(m_record_path.c_str());
            throw std::runtime_error("writing file");
        }
    
        for (int i = 0; i < m_gamerecords.count; i++) {
            ofs.write(reinterpret_cast<const char*>(&m_gamerecords.TopRecords[i]), sizeof(m_gamerecords.TopRecords[0]));
            if(!ofs){
                std::remove(m_record_path.c_str());
                throw std::runtime_error("writing file");
            }
        }
        ofs.close();
    }

    Record GetRecord () const {
        return m_gamerecords;
    }

    void LoadRecords(const std::string& records_path){
        m_record_path = records_path;
        m_gamerecords = readGameRecords();
    }

    //Evaluate New Game result then create new top 5 records. 
    void ResultInsert(GameResult new_result){
        if(m_record_path == ""){
            throw std::logic_error("records not loaded, called ResultInsert");
        }
        auto it = std::lower_bound(m_gamerecords.TopRecords.begin(), m_gamerecords.TopRecords.end(), new_result, std::greater<GameResult>());
        m_gamerecords.TopRecords.insert(it, new_result);

        if(m_gamerecords.TopRecords.size() > m_store_count){
            m_gamerecords.TopRecords.resize(m_store_count);
        }
        m_gamerecords.count = m_gamerecords.TopRecords.size();
    }
};