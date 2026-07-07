// GameManager.cpp
#include "GameManager.h"
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// ============================================================
// 构造函数和析构函数
// ============================================================

GameManager::GameManager() : m_saveFile("savegame.dat"), m_isRunning(false) {
    srand(static_cast<unsigned>(time(nullptr)));
    initializeTasks();
}

GameManager::GameManager(const string& saveFile) 
    : m_saveFile(saveFile), m_isRunning(false) {
    srand(static_cast<unsigned>(time(nullptr)));
    initializeTasks();
}

GameManager::~GameManager() {
    if (m_isRunning) {
        saveGame();
    }
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
// 游戏主循环
// ============================================================

void GameManager::run() {
    clearScreen();
    cout << BLUE << "═══════════════════════════════════════════════════" << endl;
    cout << "      🎮 校园RPG冒险游戏系统" << endl;
    cout << "═══════════════════════════════════════════════════" << RESET << endl;
    
    // 检查是否有存档
    if (hasSaveFile()) {
        cout << "📂 检测到存档文件，是否加载？(y/n): ";
        string choice;
        getline(cin, choice);
        if (toLower(choice) == "y" || toLower(choice) == "yes") {
            if (loadGame()) {
                cout << GREEN << "✅ 存档加载成功！" << RESET << endl;
            } else {
                cout << RED << "❌ 存档加载失败，将创建新角色" << RESET << endl;
                createNewCharacter();
            }
        } else {
            createNewCharacter();
        }
    } else {
        createNewCharacter();
    }
    
    m_isRunning = true;
    waitForEnter();
    
    // 主游戏循环
    while (m_isRunning) {
        clearScreen();
        cout << BLUE << "\n📊 当前状态" << RESET << endl;
        cout << "   " << m_player.getName() << " | Lv." << m_player.getLevel()
             << " | HP: " << m_player.getHp() << "/" << m_player.getMaxHp()
             << " | 金币: " << m_player.getGold() << endl;
        cout << "   EXP: " << m_player.getExp() << "/" << m_player.getMaxExp() << endl;
        
        showMainMenu();
        
        string choice;
        cout << "\n请选择操作: ";
        getline(cin, choice);
        
        if (choice == "1") {
            explore();
        } else if (choice == "2") {
            showBattleMenu();
        } else if (choice == "3") {
            showShopMenu();
        } else if (choice == "4") {
            showTaskMenu();
        } else if (choice == "5") {
            showInventoryMenu();
        } else if (choice == "6") {
            showCharacterMenu();
        } else if (choice == "7") {
            rest();
        } else if (choice == "8") {
            if (saveGame()) {
                cout << GREEN << "✅ 游戏已保存！" << RESET << endl;
            }
            waitForEnter();
        } else if (choice == "9") {
            displayStatistics();
            waitForEnter();
        } else if (choice == "0") {
            cout << "是否保存游戏并退出？(y/n): ";
            string saveChoice;
            getline(cin, saveChoice);
            if (toLower(saveChoice) == "y") {
                saveGame();
            }
            cout << "👋 感谢游玩！" << endl;
            m_isRunning = false;
        } else {
            cout << RED << "❌ 无效输入，请重新选择！" << RESET << endl;
            waitForEnter();
        }
    }
}

// ============================================================
// 菜单显示
// ============================================================

void GameManager::showMainMenu() {
    cout << "\n📋 ===== 主菜单 =====" << endl;
    cout << "  1. 探索区域" << endl;
    cout << "  2. 战斗" << endl;
    cout << "  3. 商店" << endl;
    cout << "  4. 任务" << endl;
    cout << "  5. 背包" << endl;
    cout << "  6. 角色信息" << endl;
    cout << "  7. 休息（恢复HP）" << endl;
    cout << "  8. 保存游戏" << endl;
    cout << "  9. 游戏统计" << endl;
    cout << "  0. 退出游戏" << endl;
}

// ============================================================
// 角色创建
// ============================================================

void GameManager::createNewCharacter() {
    clearScreen();
    cout << "\n👤 创建新角色" << endl;
    cout << "请输入角色名称: ";
    string name;
    getline(cin, name);
    
    if (name.empty()) {
        name = "勇者";
    }
    
    m_player = Character(name);
    cout << GREEN << "✅ 角色 " << name << " 创建成功！" << RESET << endl;
    m_player.display();
}

// ============================================================
// 探索系统
// ============================================================

void GameManager::explore() {
    clearScreen();
    cout << "\n🗺️ 探索区域..." << endl;
    
    // 随机事件
    int event = getRandomInt(0, 10);
    
    if (event < 4) {
        // 遇敌
        Enemy enemy = Enemy::createRandomEnemy();
        cout << "⚔️ 遇到了 " << enemy.getName() << "！" << endl;
        enemy.display();
        waitForEnter();
        battle(enemy);
    } else if (event < 7) {
        // 发现物品
        vector<Item> possibleItems = {
            Item("面包", "恢复20HP", ItemType::FOOD, 15, 20, 1),
            Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1),
            Item("金币袋", "发现了一些金币", ItemType::SPECIAL, 0, 0, 1)
        };
        int idx = getRandomInt(0, possibleItems.size() - 1);
        Item found = possibleItems[idx];
        
        if (found.getName() == "金币袋") {
            int gold = getRandomInt(10, 50);
            m_player.addGold(gold);
            cout << "💰 发现 " << gold << " 金币！" << endl;
            addStatistics("金币拾取", gold);
        } else {
            m_player.addItem(found);
            cout << "🎁 发现 " << found.getName() << "！" << endl;
            addStatistics("物品拾取");
        }
        waitForEnter();
    } else {
        // 什么都没发生
        cout << "🌿 这里什么也没有..." << endl;
        waitForEnter();
    }
}

// ============================================================
// 战斗系统
// ============================================================

void GameManager::showBattleMenu() {
    clearScreen();
    cout << "\n⚔️ ===== 选择敌人 =====" << endl;
    
    vector<Enemy> enemies = {
        Enemy::createSlime(),
        Enemy::createGoblin(),
        Enemy::createBoss()
    };
    
    for (int i = 0; i < (int)enemies.size(); i++) {
        cout << i + 1 << ". ";
        enemies[i].display();
        cout << endl;
    }
    cout << "0. 返回" << endl;
    
    string choice;
    cout << "\n选择要挑战的敌人 (0-3): ";
    getline(cin, choice);
    
    if (choice == "0") return;
    
    int idx = stoi(choice) - 1;
    if (idx >= 0 && idx < (int)enemies.size()) {
        battle(enemies[idx]);
    } else {
        cout << RED << "❌ 无效选择！" << RESET << endl;
        waitForEnter();
    }
}

bool GameManager::battle(Enemy& enemy) {
    clearScreen();
    cout << RED << "⚔️ ===== 战斗开始！ =====" << RESET << endl;
    cout << "敌人: " << enemy.getName() << endl;
    enemy.display();
    cout << "\n你的信息:" << endl;
    m_player.display();
    
    while (m_player.isAlive() && enemy.isAlive()) {
        cout << "\n选择行动:" << endl;
        cout << "  1. 攻击" << endl;
        cout << "  2. 使用物品" << endl;
        cout << "  3. 防御" << endl;
        cout << "  4. 逃跑" << endl;
        cout << "选择: ";
        
        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            // 玩家攻击
            int damage = max(1, m_player.getAttack() - enemy.getDefense() / 2 
                             + getRandomInt(-3, 5));
            int actualDamage = enemy.takeDamage(damage);
            cout << "💥 你对 " << enemy.getName() << " 造成 " << actualDamage << " 点伤害！" << endl;
            
            if (!enemy.isAlive()) {
                break;
            }
            
            // 敌人反击 - 传入三个参数
            enemyTurn(enemy, m_player, false);
            
        } else if (choice == "2") {
            // 使用物品
            cout << "输入要使用的物品名称: ";
            string itemName;
            getline(cin, itemName);
            useItem(itemName);
            
            if (!enemy.isAlive()) break;
            // 敌人反击 - 传入三个参数
            enemyTurn(enemy, m_player, false);
            
        } else if (choice == "3") {
            // 防御
            cout << "🛡️ 你采取了防御姿态，减少受到的伤害！" << endl;
            // 防御模式 - 第三个参数为 true
            enemyTurn(enemy, m_player, true);
            
        } else if (choice == "4") {
            // 逃跑
            int escapeChance = getRandomInt(1, 100);
            if (escapeChance > 60) {
                cout << "🏃 成功逃跑！" << endl;
                addStatistics("逃跑次数");
                waitForEnter();
                return false;
            } else {
                cout << "❌ 逃跑失败！" << endl;
                enemyTurn(enemy, m_player, false);
            }
        } else {
            cout << RED << "❌ 无效选择！" << RESET << endl;
        }
    }
    
    // 战斗结果
    if (!m_player.isAlive()) {
        cout << RED << "💀 你被击败了..." << RESET << endl;
        m_player.setHp(m_player.getMaxHp() / 2);
        int goldLoss = m_player.getGold() / 4;
        m_player.addGold(-goldLoss);
        cout << "💔 失去 " << goldLoss << " 金币，回到安全区域" << endl;
        addStatistics("死亡次数");
        waitForEnter();
        return false;
    } else if (!enemy.isAlive()) {
        cout << GREEN << "🎉 胜利！" << RESET << endl;
        int expGain = enemy.getExpReward();
        int goldGain = enemy.getGoldReward();
        m_player.addExp(expGain);
        m_player.addGold(goldGain);
        
        cout << "获得 " << expGain << " 经验值， " << goldGain << " 金币" << endl;
        addStatistics("战斗胜利");
        addStatistics("获得金币", goldGain);
        addStatistics("获得经验", expGain);
        
        updateTaskProgress(TaskType::KILL_ENEMY);
        
        if (getRandomInt(1, 100) < 30) {
            vector<Item> drops = {
                Item("面包", "恢复20HP", ItemType::FOOD, 15, 20, 1),
                Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1)
            };
            int idx = getRandomInt(0, (int)drops.size() - 1);
            m_player.addItem(drops[idx]);
            cout << "🎁 掉落物品: " << drops[idx].getName() << endl;
        }
        
        waitForEnter();
        return true;
    }
    
    return false;
}

// enemyTurn 函数定义 - 三个参数，与头文件完全匹配
void GameManager::enemyTurn(Enemy& enemy, Character& player, bool playerDefending) {
    if (!enemy.isAlive()) return;
    
    int damage = max(1, enemy.getAttack() - player.getDefense() / 2 
                     + getRandomInt(-2, 3));
    if (playerDefending) {
        damage = damage / 2;
        cout << "🛡️ 防御姿态减少了伤害！" << endl;
    }
    
    player.addHp(-damage);
    cout << "💢 " << enemy.getName() << " 对你造成 " << damage << " 点伤害！" << endl;
    
    if (!player.isAlive()) {
        cout << RED << "💀 你被击败了！" << RESET << endl;
    }
}

void GameManager::playerTurn(Enemy& enemy, Character& player) {
    // 这个函数目前未使用，保留以备扩展
}

// ============================================================
// 商店系统
// ============================================================

void GameManager::showShopMenu() {
    while (true) {
        clearScreen();
        cout << "\n🏪 ===== " << m_shop.getName() << " =====" << endl;
        cout << "💰 当前金币: " << m_player.getGold() << endl;
        m_shop.displayItems();
        
        cout << "\n操作:" << endl;
        cout << "  1. 购买物品" << endl;
        cout << "  2. 出售物品" << endl;
        cout << "  0. 离开商店" << endl;
        
        string choice;
        cout << "\n选择: ";
        getline(cin, choice);
        
        if (choice == "1") {
            cout << "输入要购买的物品名称: ";
            string itemName;
            getline(cin, itemName);
            if (itemName.empty()) continue;
            
            cout << "输入购买数量 (默认1): ";
            string qtyStr;
            getline(cin, qtyStr);
            int qty = qtyStr.empty() ? 1 : stoi(qtyStr);
            
            m_shop.buyItem(m_player, itemName, qty);
            waitForEnter();
        } else if (choice == "2") {
            cout << "输入要出售的物品名称: ";
            string itemName;
            getline(cin, itemName);
            if (itemName.empty()) continue;
            
            cout << "输入出售数量 (默认1): ";
            string qtyStr;
            getline(cin, qtyStr);
            int qty = qtyStr.empty() ? 1 : stoi(qtyStr);
            
            m_shop.sellItem(m_player, itemName, qty);
            waitForEnter();
        } else if (choice == "0") {
            break;
        } else {
            cout << RED << "❌ 无效选择！" << RESET << endl;
            waitForEnter();
        }
    }
}

// ============================================================
// 任务系统
// ============================================================

void GameManager::showTaskMenu() {
    while (true) {
        clearScreen();
        cout << "\n📜 ===== 任务系统 =====" << endl;
        
        // 显示可用任务
        cout << "\n📋 可用任务:" << endl;
        bool hasAvailable = false;
        for (const auto& task : m_availableTasks) {
            if (task.getStatus() == TaskStatus::AVAILABLE) {
                task.display();
                cout << endl;
                hasAvailable = true;
            }
        }
        if (!hasAvailable) {
            cout << "  没有可用任务" << endl;
        }
        
        // 显示已接任务
        cout << "\n⏳ 当前任务:" << endl;
        bool hasAccepted = false;
        for (const auto& task : m_acceptedTasks) {
            if (task.getStatus() == TaskStatus::ACCEPTED || 
                task.getStatus() == TaskStatus::COMPLETED) {
                task.display();
                cout << endl;
                hasAccepted = true;
            }
        }
        if (!hasAccepted) {
            cout << "  没有进行中的任务" << endl;
        }
        
        cout << "\n操作:" << endl;
        cout << "  1. 接受任务 (输入任务名称)" << endl;
        cout << "  2. 领取奖励 (输入任务名称)" << endl;
        cout << "  0. 返回" << endl;
        
        string choice;
        cout << "\n选择: ";
        getline(cin, choice);
        
        if (choice == "1") {
            cout << "输入要接受的任务名称: ";
            string taskName;
            getline(cin, taskName);
            acceptTask(taskName);
            waitForEnter();
        } else if (choice == "2") {
            cout << "输入要领取奖励的任务名称: ";
            string taskName;
            getline(cin, taskName);
            claimReward(taskName);
            waitForEnter();
        } else if (choice == "0") {
            break;
        } else {
            cout << RED << "❌ 无效选择！" << RESET << endl;
            waitForEnter();
        }
    }
}

bool GameManager::acceptTask(const string& taskName) {
    for (auto& task : m_availableTasks) {
        if (task.getName() == taskName && task.getStatus() == TaskStatus::AVAILABLE) {
            task.setStatus(TaskStatus::ACCEPTED);
            m_acceptedTasks.push_back(task);
            cout << GREEN << "✅ 接受任务: " << taskName << RESET << endl;
            return true;
        }
    }
    cout << RED << "❌ 未找到可接受的任务: " << taskName << RESET << endl;
    return false;
}

bool GameManager::claimReward(const string& taskName) {
    for (auto& task : m_acceptedTasks) {
        if (task.getName() == taskName && task.getStatus() == TaskStatus::COMPLETED) {
            m_player.addExp(task.getExpReward());
            m_player.addGold(task.getGoldReward());
            
            for (const auto& item : task.getItemRewards()) {
                m_player.addItem(item);
                cout << "🎁 获得物品: " << item.getName() << endl;
            }
            
            task.setStatus(TaskStatus::REWARDED);
            cout << GREEN << "✅ 领取奖励成功！获得 " 
                 << task.getExpReward() << " EXP, " 
                 << task.getGoldReward() << " 金币" << RESET << endl;
            
            auto it = remove_if(m_acceptedTasks.begin(), m_acceptedTasks.end(),
                [&taskName](const Task& t) { 
                    return t.getName() == taskName && t.getStatus() == TaskStatus::REWARDED;
                });
            m_acceptedTasks.erase(it, m_acceptedTasks.end());
            
            m_availableTasks.push_back(Task::createKillTask(m_player.getLevel()));
            m_availableTasks.push_back(Task::createCollectTask(m_player.getLevel()));
            
            return true;
        }
    }
    cout << RED << "❌ 未找到可领取奖励的任务: " << taskName << RESET << endl;
    return false;
}

void GameManager::updateTaskProgress(TaskType type, int amount) {
    for (auto& task : m_acceptedTasks) {
        if (task.getType() == type && task.getStatus() == TaskStatus::ACCEPTED) {
            task.addProgress(amount);
            if (task.isComplete()) {
                cout << GREEN << "✅ 任务完成: " << task.getName() << "！去领取奖励吧！" << RESET << endl;
            }
        }
    }
}

bool GameManager::completeTask(const string& taskName) {
    // 简化实现，直接调用 claimReward
    return claimReward(taskName);
}

vector<Task> GameManager::getAvailableTasks() const {
    return m_availableTasks;
}

vector<Task> GameManager::getAcceptedTasks() const {
    return m_acceptedTasks;
}

// ============================================================
// 背包系统
// ============================================================

void GameManager::showInventoryMenu() {
    clearScreen();
    cout << "\n📦 ===== 背包 =====" << endl;
    m_player.displayInventory();
    
    cout << "\n操作:" << endl;
    cout << "  输入物品名称使用该物品" << endl;
    cout << "  输入 0 返回" << endl;
    cout << "选择: ";
    
    string choice;
    getline(cin, choice);
    
    if (choice != "0" && !choice.empty()) {
        useItem(choice);
        waitForEnter();
    }
}

void GameManager::useItem(const string& itemName) {
    Item* item = m_player.getItem(itemName);
    if (!item) {
        cout << RED << "❌ 没有找到物品: " << itemName << RESET << endl;
        return;
    }
    
    if (item->getQuantity() <= 0) {
        cout << RED << "❌ 物品数量为0" << RESET << endl;
        return;
    }
    
    if (item->getType() == ItemType::FOOD || item->getType() == ItemType::MEDICINE) {
        int healAmount = item->getEffectValue();
        m_player.addHp(healAmount);
        item->addQuantity(-1);
        cout << GREEN << "✅ 使用 " << item->getName() << "，恢复 " 
             << healAmount << " HP" << RESET << endl;
        addStatistics("使用物品");
        
        if (item->getQuantity() <= 0) {
            m_player.removeItem(itemName);
        }
    } else if (item->getType() == ItemType::EQUIPMENT) {
        int bonus = item->getEffectValue();
        m_player.setAttack(m_player.getAttack() + bonus);
        item->addQuantity(-1);
        cout << GREEN << "✅ 装备 " << item->getName() << "，攻击力 +" 
             << bonus << RESET << endl;
        
        if (item->getQuantity() <= 0) {
            m_player.removeItem(itemName);
        }
    } else {
        cout << "❌ 该物品无法使用" << endl;
    }
}

// ============================================================
// 角色信息
// ============================================================

void GameManager::showCharacterMenu() {
    clearScreen();
    m_player.display();
    cout << "\n输入 0 返回: ";
    string choice;
    getline(cin, choice);
}

// ============================================================
// 休息系统
// ============================================================

void GameManager::rest() {
    int healAmount = 20 + m_player.getLevel() * 5;
    m_player.addHp(healAmount);
    cout << "💤 休息了一会，恢复 " << healAmount << " HP" << endl;
    cout << "当前 HP: " << m_player.getHp() << "/" << m_player.getMaxHp() << endl;
    waitForEnter();
}

// ============================================================
// 存档系统
// ============================================================

bool GameManager::saveGame() {
    ofstream file(m_saveFile, ios::binary);
    if (!file.is_open()) return false;
    
    // 保存角色数据
    string charData = m_player.toString();
    size_t len = charData.length();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(charData.c_str(), len);
    
    // 保存可用任务
    int taskCount = m_availableTasks.size();
    file.write(reinterpret_cast<const char*>(&taskCount), sizeof(taskCount));
    for (const auto& task : m_availableTasks) {
        string taskStr = task.toString();
        len = taskStr.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(taskStr.c_str(), len);
    }
    
    // 保存已接任务
    taskCount = m_acceptedTasks.size();
    file.write(reinterpret_cast<const char*>(&taskCount), sizeof(taskCount));
    for (const auto& task : m_acceptedTasks) {
        string taskStr = task.toString();
        len = taskStr.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(taskStr.c_str(), len);
    }
    
    file.close();
    return true;
}

bool GameManager::loadGame() {
    ifstream file(m_saveFile, ios::binary);
    if (!file.is_open()) return false;
    
    // 加载角色数据
    size_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    char* buffer = new char[len + 1];
    file.read(buffer, len);
    buffer[len] = '\0';
    m_player.loadFromString(string(buffer));
    delete[] buffer;
    
    // 加载可用任务
    m_availableTasks.clear();
    int taskCount;
    file.read(reinterpret_cast<char*>(&taskCount), sizeof(taskCount));
    for (int i = 0; i < taskCount; i++) {
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* taskBuffer = new char[len + 1];
        file.read(taskBuffer, len);
        taskBuffer[len] = '\0';
        // 简化：这里实际需要解析任务，但为了演示我们跳过
        delete[] taskBuffer;
    }
    
    // 加载已接任务
    m_acceptedTasks.clear();
    file.read(reinterpret_cast<char*>(&taskCount), sizeof(taskCount));
    for (int i = 0; i < taskCount; i++) {
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* taskBuffer = new char[len + 1];
        file.read(taskBuffer, len);
        taskBuffer[len] = '\0';
        delete[] taskBuffer;
    }
    
    file.close();
    return true;
}

bool GameManager::hasSaveFile() const {
    ifstream file(m_saveFile);
    return file.good();
}

void GameManager::stop() {
    m_isRunning = false;
}

// ============================================================
// 统计系统
// ============================================================

void GameManager::addStatistics(const string& key, int value) {
    m_statistics[key] += value;
}

void GameManager::displayStatistics() const {
    clearScreen();
    cout << "\n📊 ===== 游戏统计 =====" << endl;
    cout << "  战斗胜利: " << (m_statistics.count("战斗胜利") ? m_statistics.at("战斗胜利") : 0) << endl;
    cout << "  死亡次数: " << (m_statistics.count("死亡次数") ? m_statistics.at("死亡次数") : 0) << endl;
    cout << "  逃跑次数: " << (m_statistics.count("逃跑次数") ? m_statistics.at("逃跑次数") : 0) << endl;
    cout << "  物品拾取: " << (m_statistics.count("物品拾取") ? m_statistics.at("物品拾取") : 0) << endl;
    cout << "  使用物品: " << (m_statistics.count("使用物品") ? m_statistics.at("使用物品") : 0) << endl;
    cout << "  获得金币: " << (m_statistics.count("获得金币") ? m_statistics.at("获得金币") : 0) << endl;
    cout << "  获得经验: " << (m_statistics.count("获得经验") ? m_statistics.at("获得经验") : 0) << endl;
    cout << "  金币拾取: " << (m_statistics.count("金币拾取") ? m_statistics.at("金币拾取") : 0) << endl;
    waitForEnter();
}

// ============================================================
// 辅助函数
// ============================================================

void GameManager::checkTaskProgress(TaskType type, int amount) {
    updateTaskProgress(type, amount);
}

void GameManager::processEvents() {
    while (!m_eventQueue.empty()) {
        string event = m_eventQueue.front();
        m_eventQueue.pop();
        cout << "📢 " << event << endl;
    }
}