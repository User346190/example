#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utils.h"
#include "Item.h"
#include <map>
#include <string>

using namespace std;

class Enemy;

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
    int m_speed;
    map<string, Item> m_inventory;
    Item* m_weapon;
    Item* m_armor;
    Item* m_accessory;

public:
    Character();
    Character(const string& name);
    ~Character();

    string getName() const;
    int getLevel() const;
    int getHp() const;
    int getMaxHp() const;
    int getExp() const;
    int getMaxExp() const;
    int getGold() const;
    int getAttack() const;
    int getDefense() const;
    int getSpeed() const;
    map<string, Item>& getInventory();
    const map<string, Item>& getInventory() const;
    Item* getEquippedWeapon() const;
    Item* getEquippedArmor() const;
    Item* getEquippedAccessory() const;

    void setName(const string& name);
    void setLevel(int level);
    void setHp(int hp);
    void setMaxHp(int maxHp);
    void setExp(int exp);
    void setGold(int gold);
    void setAttack(int attack);
    void setDefense(int defense);
    void setSpeed(int speed);

    void addHp(int amount);
    void addExp(int amount);
    void addGold(int amount);
    bool isAlive() const;

    int calculateMaxExp() const;
    bool checkLevelUp();
    void levelUp();

    bool addItem(const Item& item);
    bool removeItem(const string& name, int quantity = 1);
    bool hasItem(const string& name) const;
    Item* getItem(const string& name);
    int getItemCount(const string& name) const;
    void displayInventory() const;
    int getInventorySize() const;

    bool equipItem(const string& itemName);
    bool unequipItem(ItemType slot);
    int getTotalAttack() const;
    int getTotalDefense() const;
    float getHitChance(const Enemy& target) const;
    float getDodgeChance(const Enemy& attacker) const;

    string toString() const;
    bool loadFromString(const string& data);
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);

    void display() const;
};

#endif