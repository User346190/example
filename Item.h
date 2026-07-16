#ifndef ITEM_H
#define ITEM_H

#include "Utils.h"

enum class ItemType {
    FOOD,
    MEDICINE,
    EQUIPMENT_WEAPON,
    EQUIPMENT_ARMOR,
    EQUIPMENT_ACCESSORY,
    MATERIAL,
    SPECIAL
};

class Item {
private:
    string m_name;
    string m_description;
    ItemType m_type;
    int m_price;
    int m_effectValue;
    int m_quantity;
    int m_attackBonus;
    int m_defenseBonus;
    int m_speedBonus;

public:
    Item();
    Item(const string& name, const string& desc, ItemType type, 
         int price, int effectValue, int quantity = 1,
         int attackBonus = 0, int defenseBonus = 0, int speedBonus = 0);
    
    string getName() const;
    string getDescription() const;
    ItemType getType() const;
    int getPrice() const;
    int getEffectValue() const;
    int getQuantity() const;
    string getTypeString() const;
    int getAttackBonus() const;
    int getDefenseBonus() const;
    int getSpeedBonus() const;
    
    void setQuantity(int qty);
    void addQuantity(int qty);
    
    int use();
    
    void display() const;
    string toString() const;
};

string itemTypeToString(ItemType type);

#endif