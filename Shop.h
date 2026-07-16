#ifndef SHOP_H
#define SHOP_H

#include "Utils.h"
#include "Item.h"
#include "Character.h"

class Shop {
private:
    string m_name;
    vector<Item> m_items;

public:
    Shop();
    Shop(const string& name);
    
    void addItem(const Item& item);
    void removeItem(const string& name);
    void updateItemPrice(const string& name, int newPrice);
    
    bool buyItem(Character& character, const string& itemName, int quantity = 1);
    bool sellItem(Character& character, const string& itemName, int quantity = 1);
    
    void displayItems() const;
    Item* findItem(const string& name);
    vector<Item> getItemsByType(ItemType type) const;
    
    // ---- 新增 ----
    const vector<Item>& getItems() const { return m_items; }
    
    string getName() const;
    void setName(const string& name);
    
    string toString() const;
    
private:
    int calculateBuyPrice(const Item& item) const;
    int calculateSellPrice(const Item& item) const;
};

#endif