#include "ShopDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

ShopDialog::ShopDialog(Character& player, GameManager& gameManager, QWidget* parent)
    : QDialog(parent), m_player(player), m_gameManager(gameManager) {
    qDebug() << "ShopDialog 构造函数被调用";
    setWindowTitle("🏪 商店");
    resize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_goldLabel = new QLabel("金币: " + QString::number(m_player.getGold()));
    layout->addWidget(m_goldLabel);

    m_itemList = new QListWidget;
    const auto& items = m_gameManager.getShop().getItems();
    qDebug() << "商店物品数量:" << items.size();
    for (const auto& item : items) {
        QString text = QString::fromStdString(item.getName()) + " - " +
                       QString::number(item.getPrice()) + " 金币 (库存: " +
                       QString::number(item.getQuantity()) + ")";
        m_itemList->addItem(text);
    }
    layout->addWidget(m_itemList);

    QPushButton* buyBtn = new QPushButton("购买选中物品");
    layout->addWidget(buyBtn);

    connect(buyBtn, &QPushButton::clicked, this, &ShopDialog::onBuy);
    qDebug() << "ShopDialog 构造完成";
}

void ShopDialog::onBuy() {
    int row = m_itemList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个物品");
        return;
    }
    const auto& items = m_gameManager.getShop().getItems();
    if (row >= (int)items.size()) return;
    std::string itemName = items[row].getName();
    if (m_gameManager.buyItem(itemName, 1)) {
        QMessageBox::information(this, "成功", "购买成功！");
        updateUI();
    } else {
        QMessageBox::warning(this, "失败", "购买失败，可能金币不足或库存不足");
    }
}

void ShopDialog::updateUI() {
    m_goldLabel->setText("金币: " + QString::number(m_player.getGold()));
    m_itemList->clear();
    const auto& items = m_gameManager.getShop().getItems();
    for (const auto& item : items) {
        QString text = QString::fromStdString(item.getName()) + " - " +
                       QString::number(item.getPrice()) + " 金币 (库存: " +
                       QString::number(item.getQuantity()) + ")";
        m_itemList->addItem(text);
    }
}