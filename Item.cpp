#include "Item.h"

Item::Item() : m_name(""), m_description(""), m_type(ItemType::FOOD), 
               m_price(0), m_effectValue(0), m_quantity(1),
               m_attackBonus(0), m_defenseBonus(0), m_speedBonus(0) {}

Item::Item(const string& name, const string& desc, ItemType type, 
           int price, int effectValue, int quantity,
           int attackBonus, int defenseBonus, int speedBonus)
    : m_name(name), m_description(desc), m_type(type), 
      m_price(price), m_effectValue(effectValue), m_quantity(quantity),
      m_attackBonus(attackBonus), m_defenseBonus(defenseBonus), m_speedBonus(speedBonus) {}

string Item::getName() const { return m_name; }
string Item::getDescription() const { return m_description; }
ItemType Item::getType() const { return m_type; }
int Item::getPrice() const { return m_price; }
int Item::getEffectValue() const { return m_effectValue; }
int Item::getQuantity() const { return m_quantity; }
int Item::getAttackBonus() const { return m_attackBonus; }
int Item::getDefenseBonus() const { return m_defenseBonus; }
int Item::getSpeedBonus() const { return m_speedBonus; }

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
    } else if (m_type == ItemType::EQUIPMENT_WEAPON || 
               m_type == ItemType::EQUIPMENT_ARMOR || 
               m_type == ItemType::EQUIPMENT_ACCESSORY) {
        if (m_attackBonus > 0) cout << "     攻击 +" << m_attackBonus << endl;
        if (m_defenseBonus > 0) cout << "     防御 +" << m_defenseBonus << endl;
        if (m_speedBonus > 0) cout << "     速度 +" << m_speedBonus << endl;
    }
}

string Item::toString() const {
    stringstream ss;
    ss << m_name << "|" << m_description << "|" 
       << (int)m_type << "|" << m_price << "|" 
       << m_effectValue << "|" << m_quantity << "|"
       << m_attackBonus << "|" << m_defenseBonus << "|" << m_speedBonus;
    return ss.str();
}

string itemTypeToString(ItemType type) {
    switch(type) {
        case ItemType::FOOD: return "食物";
        case ItemType::MEDICINE: return "药品";
        case ItemType::EQUIPMENT_WEAPON: return "武器";
        case ItemType::EQUIPMENT_ARMOR: return "防具";
        case ItemType::EQUIPMENT_ACCESSORY: return "饰品";
        case ItemType::MATERIAL: return "材料";
        case ItemType::SPECIAL: return "特殊";
        default: return "未知";
    }
}