// include/Item.h
#ifndef ITEM_H
#define ITEM_H

#include "Utils.h"

// 物品类型枚举
enum class ItemType {
    FOOD,       // 食物 - 恢复HP
    MEDICINE,   // 药品 - 恢复大量HP
    EQUIPMENT,  // 装备 - 增加攻击力或防御力
    MATERIAL,   // 材料 - 任务物品
    SPECIAL     // 特殊物品
};

// 物品类 - 封装
class Item {
private:
    string m_name;
    string m_description;
    ItemType m_type;
    int m_price;
    int m_effectValue;  // 效果值（恢复量/攻击加成等）
    int m_quantity;     // 数量

public:
    Item();
    Item(const string& name, const string& desc, ItemType type, 
         int price, int effectValue, int quantity = 1);
    
    // Getter方法
    string getName() const;
    string getDescription() const;
    ItemType getType() const;
    int getPrice() const;
    int getEffectValue() const;
    int getQuantity() const;
    string getTypeString() const;
    
    // Setter方法
    void setQuantity(int qty);
    void addQuantity(int qty);
    
    // 使用物品
    int use();  // 返回效果值
    
    // 显示信息
    void display() const;
    string toString() const;
};

// 物品类型转字符串
string itemTypeToString(ItemType type);

#endif