// include/Shop.h
#ifndef SHOP_H
#define SHOP_H

#include "Utils.h"
#include "Item.h"
#include "Character.h"

class Shop {
private:
    string m_name;
    vector<Item> m_items;  // 商品列表 - 使用vector存储
    map<string, int> m_priceMultiplier;  // 价格倍数

public:
    Shop();
    Shop(const string& name);
    
    // 商品管理
    void addItem(const Item& item);
    void removeItem(const string& name);
    void updateItemPrice(const string& name, int newPrice);
    
    // 交易
    bool buyItem(Character& character, const string& itemName, int quantity = 1);
    bool sellItem(Character& character, const string& itemName, int quantity = 1);
    
    // 查看
    void displayItems() const;
    Item* findItem(const string& name);
    vector<Item> getItemsByType(ItemType type) const;
    
    // 商店信息
    string getName() const;
    void setName(const string& name);
    
    // 存档
    string toString() const;
    
private:
    int calculateBuyPrice(const Item& item) const;
    int calculateSellPrice(const Item& item) const;
};

#endif