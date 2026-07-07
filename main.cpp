// main.cpp
#include"GameManager.h"


int main() {
    try {
        GameManager game;
        game.run();
    } catch (const exception& e) {
        cerr << RED << "❌ 发生异常: " << e.what() << RESET << endl;
        return 1;
    }
    
    return 0;
}