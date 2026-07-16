#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <iostream>

using namespace std;

class Character;

enum class EnemyType {
    SLIME,
    GOBLIN,
    BOSS
};

class Enemy {
private:
    string m_name;
    EnemyType m_type;
    int m_hp;
    int m_maxHp;
    int m_attack;
    int m_defense;
    int m_speed;
    int m_expReward;
    int m_goldReward;
    string m_description;

public:
    Enemy();
    Enemy(EnemyType type);
    Enemy(const string& name, EnemyType type, int hp, int attack,
          int defense, int speed, int expReward, int goldReward);

    string getName() const;
    EnemyType getType() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getDefense() const;
    int getSpeed() const;
    int getExpReward() const;
    int getGoldReward() const;
    string getDescription() const;
    string getTypeString() const;

    void setHp(int hp);
    void setSpeed(int speed);
    void addHp(int amount);
    void setDescription(const string& desc);

    bool isAlive() const;
    int takeDamage(int damage);
    int calculateDamage(const Enemy& attacker) const;

    float getHitChance(const Character& attacker) const;
    float getDodgeChance(const Character& attacker) const;

    void display() const;

    static Enemy createSlime();
    static Enemy createGoblin();
    static Enemy createBoss();
    static Enemy createRandomEnemy();
};

#endif