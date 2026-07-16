#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "Character.h"
#include "GameManager.h"

class ShopDialog : public QDialog {
    Q_OBJECT
public:
    ShopDialog(Character& player, GameManager& gameManager, QWidget* parent = nullptr);

private slots:
    void onBuy();

private:
    Character& m_player;
    GameManager& m_gameManager;
    QListWidget* m_itemList;
    QLabel* m_goldLabel;
    void updateUI();
};

#endif