#ifndef BATTLEDIALOG_H
#define BATTLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QPushButton>
#include "Character.h"
#include "Enemy.h"

class BattleDialog : public QDialog {
    Q_OBJECT
public:
    BattleDialog(Character& player, Enemy& enemy, QWidget *parent = nullptr);

signals:
    void battleFinished(bool won);

private slots:
    void onAttack();
    void onDefend();
    void onUseItem();
    void onFlee();

private:
    Character& m_player;
    Enemy& m_enemy;
    bool m_playerDefending;
    bool m_battleEnded;

    QLabel *m_playerNameLabel, *m_enemyNameLabel;
    QProgressBar *m_playerHpBar, *m_enemyHpBar;
    QLabel *m_playerHpText, *m_enemyHpText;
    QTextEdit *m_log;
    QPushButton *m_btnAttack, *m_btnDefend, *m_btnUseItem, *m_btnFlee;

    void updateUI();
    void playerTurn();
    void enemyTurn();
    void endBattle(bool won);
    void addLog(const QString& msg);
};

#endif