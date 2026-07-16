#include "Enemy.h"
#include "Character.h"
#include "Utils.h"
#include <algorithm>

Enemy::Enemy() 
    : m_name(""), m_type(EnemyType::SLIME), m_hp(0), m_maxHp(0),
      m_attack(0), m_defense(0), m_speed(5),
      m_expReward(0), m_goldReward(0), 
      m_description("") {}

Enemy::Enemy(EnemyType type) {
    switch(type) {
        case EnemyType::SLIME: *this = createSlime(); break;
        case EnemyType::GOBLIN: *this = createGoblin(); break;
        case EnemyType::BOSS: *this = createBoss(); break;
        default: *this = createSlime(); break;
    }
}

Enemy::Enemy(const string& name, EnemyType type, int hp, int attack,
             int defense, int speed, int expReward, int goldReward)
    : m_name(name), m_type(type), m_hp(hp), m_maxHp(hp),
      m_attack(attack), m_defense(defense), m_speed(speed),
      m_expReward(expReward), m_goldReward(goldReward), 
      m_description("") {}

string Enemy::getName() const { return m_name; }
EnemyType Enemy::getType() const { return m_type; }
int Enemy::getHp() const { return m_hp; }
int Enemy::getMaxHp() const { return m_maxHp; }
int Enemy::getAttack() const { return m_attack; }
int Enemy::getDefense() const { return m_defense; }
int Enemy::getSpeed() const { return m_speed; }
int Enemy::getExpReward() const { return m_expReward; }
int Enemy::getGoldReward() const { return m_goldReward; }
string Enemy::getDescription() const { return m_description; }
string Enemy::getTypeString() const {
    switch(m_type) {
        case EnemyType::SLIME: return "史莱姆";
        case EnemyType::GOBLIN: return "哥布林";
        case EnemyType::BOSS: return "Boss";
        default: return "未知";
    }
}

void Enemy::setHp(int hp) {
    m_hp = (hp > m_maxHp) ? m_maxHp : ((hp < 0) ? 0 : hp);
}

void Enemy::setSpeed(int speed) {
    m_speed = speed;
}

void Enemy::addHp(int amount) {
    m_hp += amount;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
    if (m_hp < 0) m_hp = 0;
}

void Enemy::setDescription(const string& desc) {
    m_description = desc;
}

bool Enemy::isAlive() const { return m_hp > 0; }

int Enemy::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - m_defense / 2);
    m_hp -= actualDamage;
    if (m_hp < 0) m_hp = 0;
    return actualDamage;
}

int Enemy::calculateDamage(const Enemy& attacker) const {
    int baseDamage = attacker.m_attack - m_defense / 3;
    return std::max(1, baseDamage + getRandomInt(-3, 5));
}

float Enemy::getHitChance(const Character& attacker) const {
    float base = 0.8f;
    int speedDiff = attacker.getSpeed() - m_speed;
    return std::clamp(base + speedDiff * 0.01f, 0.3f, 0.95f);
}

float Enemy::getDodgeChance(const Character& attacker) const {
    float base = 0.1f;
    int speedDiff = m_speed - attacker.getSpeed();
    return std::clamp(base + speedDiff * 0.01f, 0.05f, 0.4f);
}

void Enemy::display() const {
    cout << "👾 " << m_name << " (" << getTypeString() << ")" << endl;
    cout << "  HP: " << m_hp << "/" << m_maxHp << endl;
    cout << "  攻击: " << m_attack << " | 防御: " << m_defense << " | 速度: " << m_speed << endl;
    cout << "  奖励: " << m_expReward << " EXP, " << m_goldReward << " 金币" << endl;
    if (!m_description.empty()) {
        cout << "  " << m_description << endl;
    }
}

Enemy Enemy::createSlime() {
    Enemy e("绿色史莱姆", EnemyType::SLIME, 30, 8, 3, 5, 20, 15);
    e.m_description = "软绵绵的绿色史莱姆，看起来很弱。";
    return e;
}

Enemy Enemy::createGoblin() {
    Enemy e("哥布林战士", EnemyType::GOBLIN, 50, 15, 5, 8, 40, 30);
    e.m_description = "手持短剑的哥布林，比史莱姆强一些。";
    return e;
}

Enemy Enemy::createBoss() {
    Enemy e("暗影领主", EnemyType::BOSS, 120, 30, 10, 15, 100, 80);
    e.m_description = "强大的暗影领主，需要全力应对！";
    return e;
}

Enemy Enemy::createRandomEnemy() {
    int r = getRandomInt(0, 2);
    switch(r) {
        case 0: return createSlime();
        case 1: return createGoblin();
        case 2: return createBoss();
        default: return createSlime();
    }
}