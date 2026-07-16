#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QProgressBar>
#include <QTimer>
#include "GameManager.h"
// #include "SFMLMapRenderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GameManager& getGame() { return m_game; }
    Character& getPlayer() { return m_game.getPlayer(); }
    GameManager& getGameManager() { return m_game; }

public slots:
    void onBuyItem();
    void onBattle();
    void onShowTaskDetail();
    void onUseItem();
    void onEditCharacter();

private slots:
    void onNewGame();
    void onLoadGame();
    void onSaveGame();
    void onExplore();
    void onRest();
    void onShowMap();
    void onDeleteItem();
    void onSellItem();
    void onAcceptTask();
    void onClaimReward();
    void onMove();

private:
    GameManager m_game;
    std::string m_mapActionResult;
    // 控件指针
    QTextEdit *m_log;
    QListWidget *m_inventoryList;
    QListWidget *m_taskList;
    QLabel *m_nameLabel, *m_levelLabel, *m_hpLabel, *m_expLabel, *m_goldLabel;
    QLabel *m_attackLabel, *m_defenseLabel;
    QLabel *m_speedLabel;
    QProgressBar *m_hpBar, *m_expBar;
    QComboBox *m_itemCombo, *m_shopCombo;
    QSpinBox *m_quantitySpin;
    QLabel *m_areaLabel;
    QLabel *m_areaTypeLabel;
    QComboBox *m_moveCombo;

    // 地图渲染相关（已禁用）
    // QLabel *m_mapLabel;
    // SFMLMapRenderer *m_mapRenderer;
    // QTimer *m_mapTimer;

    void updateUI();
    void updateMapUI();
    void log(const QString& msg);
};

#endif