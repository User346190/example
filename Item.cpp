// src/Item.cpp
#include "Item.h"

Item::Item() : m_name(""), m_description(""), m_type(ItemType::FOOD), 
               m_price(0), m_effectValue(0), m_quantity(1) {}

Item::Item(const string& name, const string& desc, ItemType type, 
           int price, int effectValue, int quantity)
    : m_name(name), m_description(desc), m_type(type), 
      m_price(price), m_effectValue(effectValue), m_quantity(quantity) {}

string Item::getName() const { return m_name; }
string Item::getDescription() const { return m_description; }
ItemType Item::getType() const { return m_type; }
int Item::getPrice() const { return m_price; }
int Item::getEffectValue() const { return m_effectValue; }
int Item::getQuantity() const { return m_quantity; }

void Item::setQuantity(int qty) { 
    m_quantity = (qty > 0) ? qty : 0; 
}

void Item::addQuantity(int qty) { 
    m_quantity += qty; 
    if (m_quantity < 0) m_quantity = 0;
}

string Item::getTypeString() const {
    return itemTypeToString(m_type);
}

int Item::use() {
    if (m_quantity <= 0) return 0;
    m_quantity--;
    return m_effectValue;
}

void Item::display() const {
    cout << "  📦 " << m_name << " (x" << m_quantity << ")" << endl;
    cout << "     类型: " << getTypeString() << endl;
    cout << "     描述: " << m_description << endl;
    cout << "     价格: " << m_price << " 金币" << endl;
    if (m_type == ItemType::FOOD || m_type == ItemType::MEDICINE) {
        cout << "     效果: 恢复 " << m_effectValue << " HP" << endl;
    } else if (m_type == ItemType::EQUIPMENT) {
        cout << "     效果: 攻击 +" << m_effectValue << endl;
    }
}

string Item::toString() const {
    stringstream ss;
    ss << m_name << "|" << m_description << "|" 
       << (int)m_type << "|" << m_price << "|" 
       << m_effectValue << "|" << m_quantity;
    return ss.str();
}

string itemTypeToString(ItemType type) {
    switch(type) {
        case ItemType::FOOD: return "食物";
        case ItemType::MEDICINE: return "药品";
        case ItemType::EQUIPMENT: return "装备";
        case ItemType::MATERIAL: return "材料";
        case ItemType::SPECIAL: return "特殊";
        default: return "未知";
    }
}