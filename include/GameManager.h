// GameManager.h
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Utils.h"
#include "Character.h"
#include "Item.h"
#include "Task.h"
#include "Enemy.h"
#include "Shop.h"
#include <queue>
#include <map>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class GameManager {
private:
    Character m_player;
    Shop m_shop;
    vector<Task> m_availableTasks;
    vector<Task> m_acceptedTasks;
    queue<string> m_eventQueue;
    map<string, int> m_statistics;
    
    bool m_isRunning;
    string m_saveFile;
    
    // 私有方法
    void initializeTasks();
    void checkTaskProgress(TaskType type, int amount = 1);
    void processEvents();
    void showMainMenu();
    void showBattleMenu();
    void showShopMenu();
    void showTaskMenu();
    void showInventoryMenu();
    void showCharacterMenu();
    
    // 战斗辅助 - 声明三个参数
    bool battle(Enemy& enemy);
    void enemyTurn(Enemy& enemy, Character& player, bool playerDefending = false);
    void playerTurn(Enemy& enemy, Character& player);

public:
    GameManager();
    GameManager(const string& saveFile);
    ~GameManager();
    
    // 游戏运行
    void run();
    void stop();
    
    // 存档
    bool saveGame();
    bool loadGame();
    bool hasSaveFile() const;
    
    // 游戏操作
    void createNewCharacter();
    void explore();
    void rest();
    void useItem(const string& itemName);
    
    // 任务系统接口
    vector<Task> getAvailableTasks() const;
    vector<Task> getAcceptedTasks() const;
    bool acceptTask(const string& taskName);
    bool completeTask(const string& taskName);
    bool claimReward(const string& taskName);
    void updateTaskProgress(TaskType type, int amount = 1);
    
    // 统计
    void addStatistics(const string& key, int value = 1);
    void displayStatistics() const;
};

#endif