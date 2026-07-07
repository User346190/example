// src/Shop.cpp
#include "Shop.h"

Shop::Shop() : m_name("冒险者商店") {
    // 初始化商品
    addItem(Item("面包", "恢复20HP", ItemType::FOOD, 15, 20, 5));
    addItem(Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 3));
    addItem(Item("大生命药水", "恢复100HP", ItemType::MEDICINE, 55, 100, 2));
    addItem(Item("铁剑", "攻击+8", ItemType::EQUIPMENT, 80, 8, 1));
    addItem(Item("皮甲", "防御+5", ItemType::EQUIPMENT, 60, 5, 1));
    addItem(Item("钢盾", "防御+8", ItemType::EQUIPMENT, 100, 8, 1));
}

Shop::Shop(const string& name) : m_name(name) {
    // 同样的初始化商品
    addItem(Item("面包", "恢复20HP", ItemType::FOOD, 15, 20, 5));
    addItem(Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 3));
    addItem(Item("大生命药水", "恢复100HP", ItemType::MEDICINE, 55, 100, 2));
    addItem(Item("铁剑", "攻击+8", ItemType::EQUIPMENT, 80, 8, 1));
    addItem(Item("皮甲", "防御+5", ItemType::EQUIPMENT, 60, 5, 1));
    addItem(Item("钢盾", "防御+8", ItemType::EQUIPMENT, 100, 8, 1));
}

void Shop::addItem(const Item& item) {
    // 检查是否已存在相同物品
    for (auto& existing : m_items) {
        if (existing.getName() == item.getName()) {
            existing.addQuantity(item.getQuantity());
            return;
        }
    }
    m_items.push_back(item);
}

void Shop::removeItem(const string& name) {
    auto it = remove_if(m_items.begin(), m_items.end(),
        [&name](const Item& item) { return item.getName() == name; });
    m_items.erase(it, m_items.end());
}

void Shop::updateItemPrice(const string& name, int newPrice) {
    for (auto& item : m_items) {
        if (item.getName() == name) {
            // 通过重新创建物品来更新价格（简化的方式）
            // 实际可以使用setter
            break;
        }
    }
}

bool Shop::buyItem(Character& character, const string& itemName, int quantity) {
    Item* shopItem = findItem(itemName);
    if (!shopItem) {
        cout << RED << "❌ 商店没有这个商品！" << RESET << endl;
        return false;
    }
    
    int totalPrice = calculateBuyPrice(*shopItem) * quantity;
    
    if (character.getGold() < totalPrice) {
        cout << RED << "❌ 金币不足！需要 " << totalPrice << " 金币，当前有 " 
             << character.getGold() << " 金币" << RESET << endl;
        return false;
    }
    
    // 检查库存
    if (shopItem->getQuantity() < quantity) {
        cout << RED << "❌ 库存不足！当前库存: " << shopItem->getQuantity() << RESET << endl;
        return false;
    }
    
    // 扣钱
    character.addGold(-totalPrice);
    
    // 给物品
    Item boughtItem = *shopItem;
    boughtItem.setQuantity(quantity);
    character.addItem(boughtItem);
    
    // 减少商店库存
    shopItem->addQuantity(-quantity);
    if (shopItem->getQuantity() <= 0) {
        removeItem(itemName);
    }
    
    cout << GREEN << "✅ 成功购买 " << quantity << " 个 " << itemName 
         << "，花费 " << totalPrice << " 金币" << RESET << endl;
    return true;
}

bool Shop::sellItem(Character& character, const string& itemName, int quantity) {
    // 检查玩家是否有该物品
    if (!character.hasItem(itemName)) {
        cout << RED << "❌ 背包中没有 " << itemName << "！" << RESET << endl;
        return false;
    }
    
    Item* charItem = character.getItem(itemName);
    if (charItem->getQuantity() < quantity) {
        cout << RED << "❌ 数量不足！当前有 " << charItem->getQuantity() << RESET << endl;
        return false;
    }
    
    int totalPrice = calculateSellPrice(*charItem) * quantity;
    
    // 移除物品并给金币
    character.removeItem(itemName, quantity);
    character.addGold(totalPrice);
    
    cout << GREEN << "✅ 成功出售 " << quantity << " 个 " << itemName 
         << "，获得 " << totalPrice << " 金币" << RESET << endl;
    return true;
}

void Shop::displayItems() const {
    cout << "\n🏪 ===== " << m_name << " =====" << endl;
    if (m_items.empty()) {
        cout << "  商店暂时没有商品" << endl;
        return;
    }
    
    int i = 1;
    for (const auto& item : m_items) {
        cout << i++ << ". ";
        cout << item.getName() << " (库存: " << item.getQuantity() << ")" << endl;
        cout << "   类型: " << item.getTypeString() << " | 价格: " 
             << calculateBuyPrice(item) << " 金币" << endl;
        cout << "   效果: " << item.getDescription() << endl;
    }
}

Item* Shop::findItem(const string& name) {
    for (auto& item : m_items) {
        if (item.getName() == name) {
            return &item;
        }
    }
    return nullptr;
}

vector<Item> Shop::getItemsByType(ItemType type) const {
    vector<Item> result;
    for (const auto& item : m_items) {
        if (item.getType() == type) {
            result.push_back(item);
        }
    }
    return result;
}

string Shop::getName() const { return m_name; }
void Shop::setName(const string& name) { m_name = name; }

int Shop::calculateBuyPrice(const Item& item) const {
    return item.getPrice();
}

int Shop::calculateSellPrice(const Item& item) const {
    return item.getPrice() / 2;  // 半价回收
}

string Shop::toString() const {
    stringstream ss;
    ss << m_name << "|" << m_items.size();
    for (const auto& item : m_items) {
        ss << "|" << item.toString();
    }
    return ss.str();
}