#include "GameManager.h"
#include <algorithm>
#include <ctime>
#include <sstream>

using namespace std;

// ============================================================
// 构造函数/析构函数
// ============================================================
GameManager::GameManager() 
    : m_saveFile("savegame.dat"), m_isRunning(false), m_currentAreaId("square") {
    srand(static_cast<unsigned>(time(nullptr)));
    initializeTasks();
}

GameManager::GameManager(const string& saveFile) 
    : m_saveFile(saveFile), m_isRunning(false), m_currentAreaId("square") {
    srand(static_cast<unsigned>(time(nullptr)));
    initializeTasks();
}

GameManager::~GameManager() {}

// ============================================================
// 角色
// ============================================================
Character& GameManager::getPlayer() { return m_player; }
const Character& GameManager::getPlayer() const { return m_player; }

void GameManager::createNewCharacter(const string& name) {
    m_player = Character(name.empty() ? "勇者" : name);
    m_currentAreaId = "square";
}

// ============================================================
// 地图系统（关键函数实现）
// ============================================================
string GameManager::getCurrentAreaId() const {
    return m_currentAreaId;
}

const MapArea* GameManager::getCurrentArea() const {
    return Map::getAreaById(m_currentAreaId);
}

const MapArea* GameManager::getAreaInfo(const string& areaId) const {
    return Map::getAreaById(areaId);
}

vector<string> GameManager::getAvailableMoves() const {
    const MapArea* area = getCurrentArea();
    if (area) return area->connections;
    return vector<string>();
}

bool GameManager::moveTo(const string& areaId) {
    const MapArea* target = Map::getAreaById(areaId);
    if (!target) return false;
    const MapArea* current = getCurrentArea();
    if (!current) return false;
    // 检查目标是否在当前区域的连接列表中
    for (const string& conn : current->connections) {
        if (conn == areaId) {
            m_currentAreaId = areaId;
            return true;
        }
    }
    return false;
}

string GameManager::getAreaDescription() const {
    const MapArea* area = getCurrentArea();
    if (!area) return "未知区域";
    return area->name + " - " + area->description + "\n类型: " + Map::getAreaTypeName(area->type);
}

// ============================================================
// 敌人工厂（根据ID）
// ============================================================
Enemy GameManager::createEnemyById(const string& enemyId) {
    if (enemyId == "goblin") return Enemy::createGoblin();
    if (enemyId == "slime") return Enemy::createSlime();
    if (enemyId == "boss") return Enemy::createBoss();
    if (enemyId == "student_monster") {
        Enemy e("学生丧尸", EnemyType::GOBLIN, 40, 12, 4, 10, 35, 25);
        e.setDescription("穿着校服的丧尸，眼神空洞。");
        return e;
    }
    if (enemyId == "chemical_monster") {
        Enemy e("化学变异体", EnemyType::GOBLIN, 55, 18, 6, 12, 45, 35);
        e.setDescription("化学药剂泄漏产生的变异生物。");
        return e;
    }
    if (enemyId == "plant_monster") {
        Enemy e("食人花", EnemyType::SLIME, 35, 10, 3, 8, 25, 20);
        e.setDescription("巨大的食人花，散发着恶臭。");
        return e;
    }
    if (enemyId == "book_ghost") {
        Enemy e("书灵", EnemyType::SLIME, 25, 8, 2, 15, 20, 15);
        e.setDescription("书本中飘出的幽灵，并不危险。");
        return e;
    }
    if (enemyId == "flower_fairy") {
        Enemy e("花精", EnemyType::SLIME, 30, 9, 3, 16, 22, 18);
        e.setDescription("美丽的花精，但会攻击入侵者。");
        return e;
    }
    if (enemyId == "tree_spirit") {
        Enemy e("树精", EnemyType::SLIME, 45, 11, 5, 6, 30, 22);
        e.setDescription("古老的树精，防御很高。");
        return e;
    }
    if (enemyId == "sports_ghost") {
        Enemy e("体育幽灵", EnemyType::BOSS, 90, 25, 8, 18, 70, 55);
        e.setDescription("体育老师的幽灵，非常强大！");
        return e;
    }
    if (enemyId == "robot_player") {
        Enemy e("机器人运动员", EnemyType::BOSS, 110, 30, 10, 20, 85, 65);
        e.setDescription("全金属机器人，攻防兼备。");
        return e;
    }
    return Enemy::createSlime();
}

// ============================================================
// 探索
// ============================================================
string GameManager::explore() {
    const MapArea* area = getCurrentArea();
    if (!area) return "❌ 当前区域不存在！";

    switch (area->type) {
        case AreaType::SAFE_ZONE: {
            if (!area->npcs.empty()) {
                auto it = area->npcs.begin();
                advance(it, rand() % area->npcs.size());
                return "💬 " + it->first + "：" + it->second;
            }
            return "🏠 你在安全区，没有危险。";
        }
        case AreaType::BATTLE_ZONE: {
            if (area->enemies.empty()) return "⚔️ 这里没有敌人。";
            string enemyId = area->enemies[rand() % area->enemies.size()];
            bool win = battleEnemyById(enemyId);
            return win ? "⚔️ 击败了 " + enemyId + "！" : "💀 被 " + enemyId + " 击败...";
        }
        case AreaType::SHOP_ZONE:
            return "🏪 这里是 " + area->name + "，你可以购买物品。";
        case AreaType::EXPLORE_ZONE: {
            int r = rand() % 10;
            if (r < 4) {
                vector<Item> items = {
                    Item("面包", "恢复20HP", ItemType::FOOD, 15, 20, 1),
                    Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1),
                    Item("魔法石", "神秘物品", ItemType::SPECIAL, 0, 0, 1)
                };
                Item found = items[rand() % items.size()];
                m_player.addItem(found);
                return "🔍 在 " + area->name + " 发现了 " + found.getName() + "！";
            } else if (r < 7) {
                if (!area->enemies.empty()) {
                    string enemyId = area->enemies[rand() % area->enemies.size()];
                    bool win = battleEnemyById(enemyId);
                    return win ? "⚔️ 在探索中击败了 " + enemyId + "！" : "💀 被 " + enemyId + " 击败...";
                }
                return "🔍 你探索了 " + area->name + "，但什么也没发现。";
            } else {
                return "🔍 你在 " + area->name + " 中漫步，心情舒畅。";
            }
        }
        case AreaType::BOSS_ZONE: {
            if (area->enemies.empty()) return "👑 这里没有Boss。";
            string bossId = area->enemies[0];
            bool win = battleEnemyById(bossId);
            return win ? "👑 击败了Boss " + bossId + "！" : "💀 被Boss " + bossId + " 击败...";
        }
        case AreaType::REST_ZONE: {
            rest();
            return "💤 在 " + area->name + " 休息，HP已恢复！";
        }
        default:
            return "❓ 未知区域类型。";
    }
}

// ============================================================
// 战斗（按敌人类型）
// ============================================================
bool GameManager::battle(EnemyType enemyType) {
    Enemy enemy(enemyType);
    vector<pair<bool, int>> turnOrder;
    turnOrder.push_back({true, m_player.getSpeed()});
    turnOrder.push_back({false, enemy.getSpeed()});
    sort(turnOrder.begin(), turnOrder.end(),
         [](auto& a, auto& b) { return a.second > b.second; });

    while (m_player.isAlive() && enemy.isAlive()) {
        for (auto& turn : turnOrder) {
            if (!m_player.isAlive() || !enemy.isAlive()) break;
            
            if (turn.first) {
                float hitChance = enemy.getHitChance(m_player);
                float dodgeChance = enemy.getDodgeChance(m_player);
                if (rand() % 100 < hitChance * 100) {
                    if (rand() % 100 < dodgeChance * 100) {
                    } else {
                        int damage = max(1, m_player.getTotalAttack() - enemy.getDefense()/2 + (rand()%9-4));
                        if (rand() % 100 < 10) {
                            damage *= 1.5;
                        }
                        enemy.takeDamage(damage);
                    }
                } else {
                }
            } else {
                float hitChance = m_player.getHitChance(enemy);
                float dodgeChance = m_player.getDodgeChance(enemy);
                if (rand() % 100 < hitChance * 100) {
                    if (rand() % 100 < dodgeChance * 100) {
                    } else {
                        int damage = max(1, enemy.getAttack() - m_player.getTotalDefense()/2 + (rand()%9-4));
                        m_player.addHp(-damage);
                    }
                } else {
                }
            }
        }
    }
    if (!m_player.isAlive()) {
        m_player.setHp(m_player.getMaxHp() / 2);
        int goldLoss = m_player.getGold() / 4;
        m_player.addGold(-goldLoss);
        addStatistics("死亡次数");
        return false;
    } else {
        int expGain = enemy.getExpReward();
        int goldGain = enemy.getGoldReward();
        m_player.addExp(expGain);
        m_player.addGold(goldGain);
        addStatistics("战斗胜利");
        addStatistics("获得金币", goldGain);
        addStatistics("获得经验", expGain);
        updateTaskProgress(TaskType::KILL_ENEMY);
        if (rand() % 100 < 30) {
            Item drop("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1);
            m_player.addItem(drop);
        }
        return true;
    }
}

// ============================================================
// 战斗（按敌人ID）
// ============================================================
bool GameManager::battleEnemyById(const string& enemyId) {
    Enemy enemy = createEnemyById(enemyId);
    vector<pair<bool, int>> turnOrder;
    turnOrder.push_back({true, m_player.getSpeed()});
    turnOrder.push_back({false, enemy.getSpeed()});
    sort(turnOrder.begin(), turnOrder.end(),
         [](auto& a, auto& b) { return a.second > b.second; });

    while (m_player.isAlive() && enemy.isAlive()) {
        for (auto& turn : turnOrder) {
            if (!m_player.isAlive() || !enemy.isAlive()) break;
            
            if (turn.first) {
                float hitChance = enemy.getHitChance(m_player);
                float dodgeChance = enemy.getDodgeChance(m_player);
                if (rand() % 100 < hitChance * 100) {
                    if (rand() % 100 < dodgeChance * 100) {
                    } else {
                        int damage = max(1, m_player.getTotalAttack() - enemy.getDefense()/2 + (rand()%9-4));
                        if (rand() % 100 < 10) {
                            damage *= 1.5;
                        }
                        enemy.takeDamage(damage);
                    }
                } else {
                }
            } else {
                float hitChance = m_player.getHitChance(enemy);
                float dodgeChance = m_player.getDodgeChance(enemy);
                if (rand() % 100 < hitChance * 100) {
                    if (rand() % 100 < dodgeChance * 100) {
                    } else {
                        int damage = max(1, enemy.getAttack() - m_player.getTotalDefense()/2 + (rand()%9-4));
                        m_player.addHp(-damage);
                    }
                } else {
                }
            }
        }
    }
    if (!m_player.isAlive()) {
        m_player.setHp(m_player.getMaxHp() / 2);
        int goldLoss = m_player.getGold() / 4;
        m_player.addGold(-goldLoss);
        addStatistics("死亡次数");
        return false;
    } else {
        int expGain = enemy.getExpReward();
        int goldGain = enemy.getGoldReward();
        m_player.addExp(expGain);
        m_player.addGold(goldGain);
        addStatistics("战斗胜利");
        addStatistics("获得金币", goldGain);
        addStatistics("获得经验", expGain);
        updateTaskProgress(TaskType::KILL_ENEMY);
        if (rand() % 100 < 30) {
            Item drop("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1);
            m_player.addItem(drop);
        }
        return true;
    }
}

// ============================================================
// 商店
// ============================================================
const Shop& GameManager::getShop() const { return m_shop; }
bool GameManager::buyItem(const string& itemName, int quantity) {
    cout << "[DEBUG] GameManager::buyItem: " << itemName << " x" << quantity << endl;
    return m_shop.buyItem(m_player, itemName, quantity);
}
bool GameManager::sellItem(const string& itemName, int quantity) {
    return m_shop.sellItem(m_player, itemName, quantity);
}

// ============================================================
// 任务
// ============================================================
vector<Task> GameManager::getAvailableTasks() const { return m_availableTasks; }
vector<Task> GameManager::getAcceptedTasks() const { return m_acceptedTasks; }

bool GameManager::acceptTask(const string& taskName) {
    for (auto& task : m_availableTasks) {
        if (task.getName() == taskName && task.getStatus() == TaskStatus::AVAILABLE) {
            task.setStatus(TaskStatus::ACCEPTED);
            m_acceptedTasks.push_back(task);
            return true;
        }
    }
    return false;
}

bool GameManager::claimReward(const string& taskName) {
    for (auto it = m_acceptedTasks.begin(); it != m_acceptedTasks.end(); ++it) {
        if (it->getName() == taskName && it->getStatus() == TaskStatus::COMPLETED) {
            m_player.addExp(it->getExpReward());
            m_player.addGold(it->getGoldReward());
            for (const auto& item : it->getItemRewards()) {
                m_player.addItem(item);
            }
            m_acceptedTasks.erase(it);
            m_availableTasks.push_back(Task::createKillTask(m_player.getLevel()));
            m_availableTasks.push_back(Task::createCollectTask(m_player.getLevel()));
            return true;
        }
    }
    return false;
}

void GameManager::updateTaskProgress(TaskType type, int amount) {
    for (auto& task : m_acceptedTasks) {
        if (task.getType() == type && task.getStatus() == TaskStatus::ACCEPTED) {
            task.addProgress(amount);
        }
    }
}

// ============================================================
// 背包
// ============================================================
bool GameManager::useItem(const string& itemName) {
    Item* item = m_player.getItem(itemName);
    if (!item || item->getQuantity() <= 0) return false;
    if (item->getType() == ItemType::FOOD || item->getType() == ItemType::MEDICINE) {
        m_player.addHp(item->getEffectValue());
        item->addQuantity(-1);
        if (item->getQuantity() <= 0) m_player.removeItem(itemName);
        addStatistics("使用物品");
        return true;
    } else if (item->getType() == ItemType::EQUIPMENT_WEAPON || 
               item->getType() == ItemType::EQUIPMENT_ARMOR || 
               item->getType() == ItemType::EQUIPMENT_ACCESSORY) {
        m_player.equipItem(itemName);
        return true;
    }
    return false;
}

// ============================================================
// 休息
// ============================================================
void GameManager::rest() {
    int healAmount = 20 + m_player.getLevel() * 5;
    m_player.addHp(healAmount);
}

// ============================================================
// 存档/读档
// ============================================================
bool GameManager::saveGame() {
    return m_player.saveToFile(m_saveFile);
}
bool GameManager::loadGame() {
    return m_player.loadFromFile(m_saveFile);
}
bool GameManager::hasSaveFile() const {
    ifstream file(m_saveFile);
    return file.good();
}

// ============================================================
// 统计
// ============================================================
const map<string, int>& GameManager::getStatistics() const { return m_statistics; }
void GameManager::addStatistics(const string& key, int value) {
    m_statistics[key] += value;
}

// ============================================================
// 任务初始化
// ============================================================
void GameManager::initializeTasks() {
    m_availableTasks.clear();
    m_availableTasks.push_back(Task::createKillTask(1));
    m_availableTasks.push_back(Task::createCollectTask(1));
    m_availableTasks.push_back(Task::createLevelTask(1));
    m_availableTasks.push_back(Task::createExploreTask(1));
    m_availableTasks.push_back(Task::createKillTask(3));
    m_availableTasks.push_back(Task::createCollectTask(3));
}

// ============================================================
// 弃用方法
// ============================================================
void GameManager::run() {}
void GameManager::stop() { m_isRunning = false; }
void GameManager::processEvents() {}
void GameManager::checkTaskProgress(TaskType type, int amount) {
    updateTaskProgress(type, amount);
}