#include <iostream>
#include <cassert>
#include "../src/GameRecords.h"

int main(){
    std::string filename = "RecordTest.rec";
    std::remove(filename.c_str());
    GameRecords test1;
    test1.LoadRecords(filename);
    assert(test1.GetRecord().count == 0);
    test1.ResultInsert({1});
    test1.writeToFile();

    GameRecords read;
    read.LoadRecords(filename);
    assert(read.GetRecord().count == 1);
    assert(read.GetRecord().TopRecords[0].point == 1);

    test1.ResultInsert({2});
    test1.ResultInsert({3});
    test1.ResultInsert({4});
    test1.ResultInsert({5});
    test1.ResultInsert({6});

    assert(test1.GetRecord().count == 5);
    assert(test1.GetRecord().TopRecords[0].point == 6);

    std::remove(filename.c_str());
}
