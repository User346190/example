#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Utils.h"
#include "Character.h"
#include "Item.h"
#include "Task.h"
#include "Enemy.h"
#include "Shop.h"
#include "Map.h"
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
    string m_currentAreaId;

    void initializeTasks();
    void processEvents();
    void checkTaskProgress(TaskType type, int amount = 1);
    Enemy createEnemyById(const string& enemyId);

public:
    GameManager();
    GameManager(const string& saveFile);
    ~GameManager();

    // ---- 角色管理 ----
    Character& getPlayer();
    const Character& getPlayer() const;
    void createNewCharacter(const string& name);

    // ---- 地图系统 ----
    string getCurrentAreaId() const;
    const MapArea* getCurrentArea() const;
    const MapArea* getAreaInfo(const string& areaId) const;
    vector<string> getAvailableMoves() const;
    bool moveTo(const string& areaId);
    string getAreaDescription() const;

    // ---- 探索与战斗 ----
    string explore();
    bool battle(EnemyType enemyType);
    bool battleEnemyById(const string& enemyId);

    // ---- 商店 ----
    const Shop& getShop() const;
    bool buyItem(const string& itemName, int quantity = 1);
    bool sellItem(const string& itemName, int quantity = 1);

    // ---- 任务 ----
    vector<Task> getAvailableTasks() const;
    vector<Task> getAcceptedTasks() const;
    bool acceptTask(const string& taskName);
    bool claimReward(const string& taskName);

    // ---- 背包 ----
    bool useItem(const string& itemName);

    // ---- 休息 ----
    void rest();

    // ---- 存档 ----
    bool saveGame();
    bool loadGame();
    bool hasSaveFile() const;

    // ---- 统计与任务进度（公开给界面） ----
    const map<string, int>& getStatistics() const;
    void addStatistics(const string& key, int value = 1);          // 现在 public
    void updateTaskProgress(TaskType type, int amount = 1);         // 现在 public

    // ---- 弃用 ----
    void run();
    void stop();
};

#endif