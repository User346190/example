// include/Character.h
#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utils.h"
#include "Item.h"

class Character {
private:
    string m_name;
    int m_level;
    int m_hp;
    int m_maxHp;
    int m_exp;
    int m_maxExp;
    int m_gold;
    int m_attack;
    int m_defense;
    map<string, Item> m_inventory;  // 背包 - 使用map存储

public:
    Character();
    Character(const string& name);
    
    // Getter方法
    string getName() const;
    int getLevel() const;
    int getHp() const;
    int getMaxHp() const;
    int getExp() const;
    int getMaxExp() const;
    int getGold() const;
    int getAttack() const;
    int getDefense() const;
    map<string, Item>& getInventory();
    
    // Setter方法
    void setName(const string& name);
    void setLevel(int level);
    void setHp(int hp);
    void setMaxHp(int maxHp);
    void setExp(int exp);
    void setGold(int gold);
    void setAttack(int attack);
    void setDefense(int defense);
    
    // 属性操作
    void addHp(int amount);
    void addExp(int amount);
    void addGold(int amount);
    bool isAlive() const;
    
    // 经验值与升级
    int calculateMaxExp() const;
    bool checkLevelUp();
    void levelUp();
    
    // 背包管理
    bool addItem(const Item& item);
    bool removeItem(const string& name, int quantity = 1);
    bool hasItem(const string& name) const;
    Item* getItem(const string& name);
    int getItemCount(const string& name) const;
    void displayInventory() const;
    int getInventorySize() const;
    
    // 存档与读档
    string toString() const;
    bool loadFromString(const string& data);
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
    
    // 显示角色信息
    void display() const;
};

#endif