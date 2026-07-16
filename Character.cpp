#include "Character.h"
#include "Enemy.h"

Character::Character() 
    : m_name(""), m_level(1), m_hp(100), m_maxHp(100), 
      m_exp(0), m_maxExp(100), m_gold(0), m_attack(10), m_defense(5), m_speed(10),
      m_weapon(nullptr), m_armor(nullptr), m_accessory(nullptr) {}

Character::Character(const string& name)
    : m_name(name), m_level(1), m_hp(100), m_maxHp(100),
      m_exp(0), m_maxExp(100), m_gold(100), m_attack(10), m_defense(5), m_speed(10),
      m_weapon(nullptr), m_armor(nullptr), m_accessory(nullptr) {}

Character::~Character() {
    delete m_weapon;
    delete m_armor;
    delete m_accessory;
}

string Character::getName() const { return m_name; }
int Character::getLevel() const { return m_level; }
int Character::getHp() const { return m_hp; }
int Character::getMaxHp() const { return m_maxHp; }
int Character::getExp() const { return m_exp; }
int Character::getMaxExp() const { return m_maxExp; }
int Character::getGold() const { return m_gold; }
int Character::getAttack() const { return m_attack; }
int Character::getDefense() const { return m_defense; }
int Character::getSpeed() const { return m_speed; }
map<string, Item>& Character::getInventory() { return m_inventory; }
Item* Character::getEquippedWeapon() const { return m_weapon; }
Item* Character::getEquippedArmor() const { return m_armor; }
Item* Character::getEquippedAccessory() const { return m_accessory; }

void Character::setName(const string& name) { m_name = name; }
void Character::setLevel(int level) { m_level = level; }
void Character::setHp(int hp) {
    if (hp > m_maxHp) {
        m_maxHp = hp;
    }
    m_hp = hp;
}
void Character::setMaxHp(int maxHp) { m_maxHp = maxHp; }
void Character::setExp(int exp) { m_exp = exp; }
void Character::setGold(int gold) { m_gold = (gold < 0) ? 0 : gold; }
void Character::setAttack(int attack) { m_attack = attack; }
void Character::setDefense(int defense) { m_defense = defense; }
void Character::setSpeed(int speed) { m_speed = speed; }

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
    return 100 + m_level * 50;
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
    m_speed += 2;
    cout << GREEN << "🎉 升级！当前等级: " << m_level << RESET << endl;
    cout << "   HP上限: " << m_maxHp << " | 攻击: " << m_attack 
         << " | 防御: " << m_defense << " | 速度: " << m_speed << endl;
}

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

int Character::getTotalAttack() const {
    int bonus = 0;
    if (m_weapon) bonus += m_weapon->getAttackBonus();
    if (m_accessory) bonus += m_accessory->getAttackBonus();
    return m_attack + bonus;
}

int Character::getTotalDefense() const {
    int bonus = 0;
    if (m_armor) bonus += m_armor->getDefenseBonus();
    if (m_accessory) bonus += m_accessory->getDefenseBonus();
    return m_defense + bonus;
}

bool Character::equipItem(const string& itemName) {
    Item* item = getItem(itemName);
    if (!item) return false;
    ItemType type = item->getType();
    if (type != ItemType::EQUIPMENT_WEAPON && 
        type != ItemType::EQUIPMENT_ARMOR && 
        type != ItemType::EQUIPMENT_ACCESSORY) {
        return false;
    }

    Item** slot = nullptr;
    if (type == ItemType::EQUIPMENT_WEAPON) slot = &m_weapon;
    else if (type == ItemType::EQUIPMENT_ARMOR) slot = &m_armor;
    else if (type == ItemType::EQUIPMENT_ACCESSORY) slot = &m_accessory;
    
    if (*slot) {
        addItem(**slot);
        delete *slot;
    }
    *slot = new Item(*item);
    removeItem(itemName, 1);
    return true;
}

bool Character::unequipItem(ItemType slot) {
    Item** itemSlot = nullptr;
    if (slot == ItemType::EQUIPMENT_WEAPON) itemSlot = &m_weapon;
    else if (slot == ItemType::EQUIPMENT_ARMOR) itemSlot = &m_armor;
    else if (slot == ItemType::EQUIPMENT_ACCESSORY) itemSlot = &m_accessory;
    
    if (!itemSlot || !*itemSlot) return false;
    
    addItem(**itemSlot);
    delete *itemSlot;
    *itemSlot = nullptr;
    return true;
}

float Character::getHitChance(const Enemy& target) const {
    float base = 0.8f;
    int speedDiff = m_speed - target.getSpeed();
    return std::clamp(base + speedDiff * 0.01f, 0.3f, 0.95f);
}

float Character::getDodgeChance(const Enemy& attacker) const {
    float base = 0.1f;
    int speedDiff = m_speed - attacker.getSpeed();
    return std::clamp(base + speedDiff * 0.01f, 0.05f, 0.4f);
}

string Character::toString() const {
    stringstream ss;
    ss << m_name << "|" << m_level << "|" << m_hp << "|" << m_maxHp << "|"
       << m_exp << "|" << m_gold << "|" << m_attack << "|" << m_defense << "|" << m_speed;
    return ss.str();
}

bool Character::loadFromString(const string& data) {
    stringstream ss(data);
    string token;
    vector<string> tokens;
    while (getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() < 9) return false;
    
    m_name = tokens[0];
    m_level = stoi(tokens[1]);
    m_hp = stoi(tokens[2]);
    m_maxHp = stoi(tokens[3]);
    m_exp = stoi(tokens[4]);
    m_gold = stoi(tokens[5]);
    m_attack = stoi(tokens[6]);
    m_defense = stoi(tokens[7]);
    m_speed = (tokens.size() > 8) ? stoi(tokens[8]) : 10;
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
    cout << "  攻击: " << m_attack << " (总攻击: " << getTotalAttack() << ")" << endl;
    cout << "  防御: " << m_defense << " (总防御: " << getTotalDefense() << ")" << endl;
    cout << "  速度: " << m_speed << endl;
    cout << "  背包物品数: " << getInventorySize() << endl;
    
    if (m_weapon || m_armor || m_accessory) {
        cout << "  装备:" << endl;
        if (m_weapon) cout << "    武器: " << m_weapon->getName() << endl;
        if (m_armor) cout << "    防具: " << m_armor->getName() << endl;
        if (m_accessory) cout << "    饰品: " << m_accessory->getName() << endl;
    }
}