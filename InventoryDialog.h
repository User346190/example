#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "Character.h"

class InventoryDialog : public QDialog {
    Q_OBJECT
public:
    InventoryDialog(Character& player, QWidget* parent = nullptr);

signals:
    void equipmentChanged();

private slots:
    void onUse();
    void onDelete();
    void onEquip();

private:
    Character& m_player;
    QListWidget* m_itemList;
    QLabel* m_infoLabel;
    void updateUI();
};

#endif