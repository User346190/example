#include "InventoryDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

InventoryDialog::InventoryDialog(Character& player, QWidget* parent)
    : QDialog(parent), m_player(player) {
    setWindowTitle("🎒 背包");
    resize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_infoLabel = new QLabel("背包物品");
    layout->addWidget(m_infoLabel);

    m_itemList = new QListWidget;
    for (const auto& pair : m_player.getInventory()) {
        QString text = QString::fromStdString(pair.first) + " x" + QString::number(pair.second.getQuantity());
        m_itemList->addItem(text);
    }
    layout->addWidget(m_itemList);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    QPushButton* useBtn = new QPushButton("使用");
    QPushButton* delBtn = new QPushButton("删除");
    QPushButton* equipBtn = new QPushButton("装备");
    btnLayout->addWidget(useBtn);
    btnLayout->addWidget(delBtn);
    btnLayout->addWidget(equipBtn);
    layout->addLayout(btnLayout);

    connect(useBtn, &QPushButton::clicked, this, &InventoryDialog::onUse);
    connect(delBtn, &QPushButton::clicked, this, &InventoryDialog::onDelete);
    connect(equipBtn, &QPushButton::clicked, this, &InventoryDialog::onEquip);
}

void InventoryDialog::onUse() {
    int row = m_itemList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个物品");
        return;
    }
    auto it = m_player.getInventory().begin();
    std::advance(it, row);
    std::string itemName = it->first;
    if (m_player.getItem(itemName)->getQuantity() > 0) {
        if (itemName == "面包" || itemName == "生命药水") {
            m_player.addHp(30);
            m_player.removeItem(itemName, 1);
            QMessageBox::information(this, "使用", "使用了 " + QString::fromStdString(itemName) + "，恢复30HP");
            updateUI();
        } else {
            QMessageBox::information(this, "提示", "该物品无法使用");
        }
    } else {
        QMessageBox::warning(this, "错误", "物品数量不足");
    }
}

void InventoryDialog::onDelete() {
    int row = m_itemList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个物品");
        return;
    }
    auto it = m_player.getInventory().begin();
    std::advance(it, row);
    std::string itemName = it->first;
    if (m_player.removeItem(itemName, 1)) {
        QMessageBox::information(this, "删除", "删除了 " + QString::fromStdString(itemName));
        updateUI();
    } else {
        QMessageBox::warning(this, "错误", "删除失败");
    }
}

void InventoryDialog::onEquip() {
    int row = m_itemList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个物品");
        return;
    }
    auto it = m_player.getInventory().begin();
    std::advance(it, row);
    std::string itemName = it->first;
    if (m_player.equipItem(itemName)) {
        QMessageBox::information(this, "成功", "装备成功！");
        updateUI();
        emit equipmentChanged();
    } else {
        QMessageBox::warning(this, "失败", "无法装备该物品（非装备类或背包无此物品）");
    }
}

void InventoryDialog::updateUI() {
    m_itemList->clear();
    for (const auto& pair : m_player.getInventory()) {
        QString text = QString::fromStdString(pair.first) + " x" + QString::number(pair.second.getQuantity());
        m_itemList->addItem(text);
    }
}