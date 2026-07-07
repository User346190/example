// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <iostream>

using namespace std;

// 敌人类型枚举
enum class EnemyType {
    SLIME,      // 史莱姆 - 弱
    GOBLIN,     // 哥布林 - 中等
    BOSS        // Boss - 强
};

class Enemy {
private:
    string m_name;
    EnemyType m_type;
    int m_hp;
    int m_maxHp;
    int m_attack;
    int m_defense;
    int m_expReward;
    int m_goldReward;
    string m_description;

public:
    // 构造函数
    Enemy();
    Enemy(EnemyType type);
    Enemy(const string& name, EnemyType type, int hp, int attack, 
          int defense, int expReward, int goldReward);
    
    // Getter方法
    string getName() const;
    EnemyType getType() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getDefense() const;
    int getExpReward() const;
    int getGoldReward() const;
    string getDescription() const;
    string getTypeString() const;
    
    // Setter方法
    void setHp(int hp);
    void addHp(int amount);
    
    // 战斗相关
    bool isAlive() const;
    int takeDamage(int damage);
    int calculateDamage(const Enemy& attacker) const;
    
    // 显示
    void display() const;
    
    // 工厂方法 - 创建不同类型的敌人
    static Enemy createSlime();
    static Enemy createGoblin();
    static Enemy createBoss();
    static Enemy createRandomEnemy();
};

#endif