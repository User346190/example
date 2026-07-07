// src/Character.cpp
#include "Character.h"

Character::Character() 
    : m_name(""), m_level(1), m_hp(100), m_maxHp(100), 
      m_exp(0), m_maxExp(100), m_gold(0), m_attack(10), m_defense(5) {}

Character::Character(const string& name)
    : m_name(name), m_level(1), m_hp(100), m_maxHp(100),
      m_exp(0), m_maxExp(100), m_gold(100), m_attack(10), m_defense(5) {}

// Getter实现
string Character::getName() const { return m_name; }
int Character::getLevel() const { return m_level; }
int Character::getHp() const { return m_hp; }
int Character::getMaxHp() const { return m_maxHp; }
int Character::getExp() const { return m_exp; }
int Character::getMaxExp() const { return m_maxExp; }
int Character::getGold() const { return m_gold; }
int Character::getAttack() const { return m_attack; }
int Character::getDefense() const { return m_defense; }
map<string, Item>& Character::getInventory() { return m_inventory; }

// Setter实现
void Character::setName(const string& name) { m_name = name; }
void Character::setLevel(int level) { m_level = level; }
void Character::setHp(int hp) { m_hp = (hp > m_maxHp) ? m_maxHp : ((hp < 0) ? 0 : hp); }
void Character::setMaxHp(int maxHp) { m_maxHp = maxHp; }
void Character::setExp(int exp) { m_exp = exp; }
void Character::setGold(int gold) { m_gold = (gold < 0) ? 0 : gold; }
void Character::setAttack(int attack) { m_attack = attack; }
void Character::setDefense(int defense) { m_defense = defense; }

void Character::addHp(int amount) {
    m_hp += amount;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
    if (m_hp < 0) m_hp = 0;
}

void Character::addExp(int amount) {
    m_exp += amount;
    while (m_exp >= m_maxExp) {
        m_exp -= m_maxExp;
        levelUp();
    }
}

void Character::addGold(int amount) {
    m_gold += amount;
    if (m_gold < 0) m_gold = 0;
}

bool Character::isAlive() const { return m_hp > 0; }

int Character::calculateMaxExp() const {
    return 100 + m_level * 50;  // 每级需要的经验值递增
}

bool Character::checkLevelUp() {
    if (m_exp >= m_maxExp) {
        m_exp -= m_maxExp;
        levelUp();
        return true;
    }
    return false;
}

void Character::levelUp() {
    m_level++;
    m_maxExp = calculateMaxExp();
    m_maxHp += 20;
    m_hp = m_maxHp;
    m_attack += 5;
    m_defense += 3;
    cout << GREEN << "🎉 升级！当前等级: " << m_level << RESET << endl;
    cout << "   HP上限: " << m_maxHp << " | 攻击: " << m_attack 
         << " | 防御: " << m_defense << endl;
}

// 背包管理
bool Character::addItem(const Item& item) {
    string key = item.getName();
    auto it = m_inventory.find(key);
    if (it != m_inventory.end()) {
        it->second.addQuantity(item.getQuantity());
    } else {
        m_inventory[key] = item;
    }
    return true;
}

bool Character::removeItem(const string& name, int quantity) {
    auto it = m_inventory.find(name);
    if (it == m_inventory.end()) return false;
    
    if (it->second.getQuantity() <= quantity) {
        m_inventory.erase(it);
    } else {
        it->second.addQuantity(-quantity);
    }
    return true;
}

bool Character::hasItem(const string& name) const {
    return m_inventory.find(name) != m_inventory.end();
}

Item* Character::getItem(const string& name) {
    auto it = m_inventory.find(name);
    if (it != m_inventory.end()) {
        return &(it->second);
    }
    return nullptr;
}

int Character::getItemCount(const string& name) const {
    auto it = m_inventory.find(name);
    if (it != m_inventory.end()) {
        return it->second.getQuantity();
    }
    return 0;
}

void Character::displayInventory() const {
    if (m_inventory.empty()) {
        cout << "📭 背包是空的" << endl;
        return;
    }
    
    cout << "\n📦 ===== 背包 (共 " << m_inventory.size() << " 种物品) =====" << endl;
    int i = 1;
    for (const auto& pair : m_inventory) {
        cout << i++ << ". ";
        pair.second.display();
        cout << endl;
    }
}

int Character::getInventorySize() const {
    int total = 0;
    for (const auto& pair : m_inventory) {
        total += pair.second.getQuantity();
    }
    return total;
}

// 存档
string Character::toString() const {
    stringstream ss;
    ss << m_name << "|" << m_level << "|" << m_hp << "|" << m_maxHp << "|"
       << m_exp << "|" << m_gold << "|" << m_attack << "|" << m_defense;
    return ss.str();
}

bool Character::loadFromString(const string& data) {
    stringstream ss(data);
    string token;
    vector<string> tokens;
    while (getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() < 8) return false;
    
    m_name = tokens[0];
    m_level = stoi(tokens[1]);
    m_hp = stoi(tokens[2]);
    m_maxHp = stoi(tokens[3]);
    m_exp = stoi(tokens[4]);
    m_gold = stoi(tokens[5]);
    m_attack = stoi(tokens[6]);
    m_defense = stoi(tokens[7]);
    m_maxExp = calculateMaxExp();
    return true;
}

bool Character::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;
    file << toString() << endl;
    file.close();
    return true;
}

bool Character::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    string data;
    getline(file, data);
    file.close();
    return loadFromString(data);
}

void Character::display() const {
    cout << "\n👤 ===== 角色信息 =====" << endl;
    cout << "  姓名: " << m_name << endl;
    cout << "  等级: " << m_level << endl;
    cout << "  HP: " << m_hp << "/" << m_maxHp << endl;
    cout << "  经验: " << m_exp << "/" << m_maxExp << endl;
    cout << "  金币: " << m_gold << endl;
    cout << "  攻击: " << m_attack << endl;
    cout << "  防御: " << m_defense << endl;
    cout << "  背包物品数: " << getInventorySize() << endl;
}