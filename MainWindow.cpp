#include "MainWindow.h"
#include "Map.h"
#include "SFMLMapWindow.h"

#include "EditCharacterDialog.h"
#include "BattleDialog.h"
#include "TaskDetailDialog.h"
#include "ShopDialog.h"
#include "InventoryDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QTimer>
#include <QDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // ---- 全局样式 ----
    this->setStyleSheet(R"(
        QMainWindow { background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #1a1a2e, stop:1 #16213e); }
        QGroupBox { color: #e0e0e0; border: 1px solid #2a2a4a; border-radius: 15px; margin-top: 15px; padding-top: 10px; background: rgba(255,255,255,0.05); }
        QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 10px; }
        QLabel { color: #c8d6e5; font-size: 14px; }
        QPushButton { background-color: #00d2d3; color: #1a1a2e; border: none; border-radius: 20px; padding: 8px 16px; font-weight: bold; }
        QPushButton:hover { background-color: #01a3a4; color: white; }
        QListWidget, QComboBox, QTextEdit { background-color: rgba(0,0,0,0.3); color: #f5f6fa; border: 1px solid #2a2a4a; border-radius: 10px; padding: 5px; }
        QTextEdit { font-family: monospace; }
        QProgressBar { background-color: #2a2a4a; border-radius: 10px; text-align: center; color: white; }
        QProgressBar::chunk { border-radius: 10px; }
        QSpinBox {
            background-color: rgba(0,0,0,0.4);
            color: #ffffff;
            border: 1px solid #4a4a7a;
            border-radius: 8px;
            padding: 4px 22px 4px 8px;
            min-width: 60px;
            font-weight: bold;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            width: 16px;
            background-color: #2a2a4a;
            border: none;
            border-radius: 4px;
        }
    )");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // ---- 角色信息区域 ----
    QGroupBox *infoBox = new QGroupBox("📋 角色信息");
    QGridLayout *grid = new QGridLayout(infoBox);
    m_nameLabel = new QLabel; m_levelLabel = new QLabel;
    m_hpLabel = new QLabel; m_expLabel = new QLabel;
    m_goldLabel = new QLabel; m_attackLabel = new QLabel;
    m_defenseLabel = new QLabel; m_speedLabel = new QLabel;

    m_hpBar = new QProgressBar;
    m_hpBar->setRange(0, 100);
    m_hpBar->setValue(100);
    m_hpBar->setFormat("%v/%m");
    m_expBar = new QProgressBar;
    m_expBar->setRange(0, 100);
    m_expBar->setValue(0);
    m_expBar->setFormat("%v/%m");

    grid->addWidget(new QLabel("姓名:"), 0, 0); grid->addWidget(m_nameLabel, 0, 1);
    grid->addWidget(new QLabel("等级:"), 0, 2); grid->addWidget(m_levelLabel, 0, 3);
    grid->addWidget(new QLabel("HP:"), 1, 0); grid->addWidget(m_hpBar, 1, 1, 1, 3);
    grid->addWidget(new QLabel("经验:"), 2, 0); grid->addWidget(m_expBar, 2, 1, 1, 3);
    grid->addWidget(new QLabel("金币:"), 3, 0); grid->addWidget(m_goldLabel, 3, 1);
    grid->addWidget(new QLabel("速度:"), 3, 2); grid->addWidget(m_speedLabel, 3, 3);
    grid->addWidget(new QLabel("攻击:"), 4, 0); grid->addWidget(m_attackLabel, 4, 1);
    grid->addWidget(new QLabel("防御:"), 4, 2); grid->addWidget(m_defenseLabel, 4, 3);
    mainLayout->addWidget(infoBox);

    // ---- 地图区域（移动控制） ----
    QGroupBox *mapBox = new QGroupBox("🗺️ 当前位置 & 移动");
    QHBoxLayout *mapLayout = new QHBoxLayout(mapBox);
    m_areaLabel = new QLabel("中央广场");
    mapLayout->addWidget(new QLabel("当前区域:"));
    mapLayout->addWidget(m_areaLabel);
    mapLayout->addStretch();
    m_areaTypeLabel = new QLabel("🏠 安全区");
    mapLayout->addWidget(m_areaTypeLabel);
    mapLayout->addStretch();
    m_moveCombo = new QComboBox;
    m_moveCombo->setMinimumWidth(150);
    mapLayout->addWidget(new QLabel("移动到:"));
    mapLayout->addWidget(m_moveCombo);
    QPushButton *btnMove = new QPushButton("移动");
    mapLayout->addWidget(btnMove);
    mainLayout->addWidget(mapBox);

    // ---- 操作按钮 ----
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *btnNew = new QPushButton("新游戏");
    QPushButton *btnLoad = new QPushButton("读档");
    QPushButton *btnSave = new QPushButton("存档");
    QPushButton *btnExplore = new QPushButton("探索");
    QPushButton *btnBattle = new QPushButton("战斗");
    QPushButton *btnRest = new QPushButton("休息");
    QPushButton *btnEditChar = new QPushButton("✏️ 编辑角色");
    QPushButton *btnShowMap = new QPushButton("🗺️ 地图");
    btnLayout->addWidget(btnNew);
    btnLayout->addWidget(btnLoad);
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExplore);
    btnLayout->addWidget(btnBattle);
    btnLayout->addWidget(btnRest);
    btnLayout->addWidget(btnEditChar);
    btnLayout->addWidget(btnShowMap);
    mainLayout->addLayout(btnLayout);

    // ---- 背包与商店 ----
    QHBoxLayout *middleLayout = new QHBoxLayout;

    QGroupBox *inventoryBox = new QGroupBox("🎒 背包");
    QVBoxLayout *invLayout = new QVBoxLayout(inventoryBox);
    m_inventoryList = new QListWidget;
    invLayout->addWidget(m_inventoryList);
    QHBoxLayout *useLayout = new QHBoxLayout;
    m_itemCombo = new QComboBox;
    useLayout->addWidget(m_itemCombo);
    QPushButton *btnUse = new QPushButton("使用");
    QPushButton *btnDelete = new QPushButton("🗑️ 删除");
    useLayout->addWidget(btnUse);
    useLayout->addWidget(btnDelete);
    invLayout->addLayout(useLayout);
    middleLayout->addWidget(inventoryBox);

    QGroupBox *shopBox = new QGroupBox("🏪 商店");
    QVBoxLayout *shopLayout = new QVBoxLayout(shopBox);
    m_shopCombo = new QComboBox;
    shopLayout->addWidget(m_shopCombo);
    QHBoxLayout *shopBtns = new QHBoxLayout;
    m_quantitySpin = new QSpinBox;
    m_quantitySpin->setRange(1, 99);
    m_quantitySpin->setValue(1);
    m_quantitySpin->setFixedWidth(60);
    shopBtns->addWidget(new QLabel("数量:"));
    shopBtns->addWidget(m_quantitySpin);
    QPushButton *btnBuy = new QPushButton("购买");
    QPushButton *btnSell = new QPushButton("出售");
    shopBtns->addWidget(btnBuy);
    shopBtns->addWidget(btnSell);
    shopLayout->addLayout(shopBtns);
    middleLayout->addWidget(shopBox);
    mainLayout->addLayout(middleLayout);

    // ---- 任务 ----
    QGroupBox *taskBox = new QGroupBox("📜 任务");
    QVBoxLayout *taskLayout = new QVBoxLayout(taskBox);
    m_taskList = new QListWidget;
    taskLayout->addWidget(m_taskList);
    QHBoxLayout *taskBtns = new QHBoxLayout;
    QPushButton *btnAccept = new QPushButton("接受任务");
    QPushButton *btnClaim = new QPushButton("领取奖励");
    QPushButton *btnTaskDetail = new QPushButton("📋 查看详情");
    taskBtns->addWidget(btnAccept);
    taskBtns->addWidget(btnClaim);
    taskBtns->addWidget(btnTaskDetail);
    taskLayout->addLayout(taskBtns);
    mainLayout->addWidget(taskBox);

    // ---- 日志 ----
    QGroupBox *logBox = new QGroupBox("📜 日志");
    QVBoxLayout *logLayout = new QVBoxLayout(logBox);
    m_log = new QTextEdit;
    m_log->setReadOnly(true);
    logLayout->addWidget(m_log);
    mainLayout->addWidget(logBox);

    // ---- 信号连接 ----
    connect(btnNew, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadGame);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveGame);
    connect(btnExplore, &QPushButton::clicked, this, &MainWindow::onExplore);
    connect(btnBattle, &QPushButton::clicked, this, &MainWindow::onBattle);
    connect(btnRest, &QPushButton::clicked, this, &MainWindow::onRest);
    connect(btnEditChar, &QPushButton::clicked, this, &MainWindow::onEditCharacter);
    connect(btnShowMap, &QPushButton::clicked, this, &MainWindow::onShowMap);
    connect(btnUse, &QPushButton::clicked, this, &MainWindow::onUseItem);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteItem);
    connect(btnBuy, &QPushButton::clicked, this, &MainWindow::onBuyItem);
    connect(btnSell, &QPushButton::clicked, this, &MainWindow::onSellItem);
    connect(btnAccept, &QPushButton::clicked, this, &MainWindow::onAcceptTask);
    connect(btnClaim, &QPushButton::clicked, this, &MainWindow::onClaimReward);
    connect(btnTaskDetail, &QPushButton::clicked, this, &MainWindow::onShowTaskDetail);
    connect(btnMove, &QPushButton::clicked, this, &MainWindow::onMove);

    // ---- 初始化地图渲染器（已禁用）----
    // m_mapLabel = new QLabel;
    // m_mapLabel->setFixedSize(800, 600);
    // m_mapLabel->setScaledContents(false);
    // m_mapRenderer = new SFMLMapRenderer(800, 600);
    // m_mapRenderer->setCurrentArea(m_game.getCurrentAreaId());
    // m_mapTimer = new QTimer(this);
    // connect(m_mapTimer, &QTimer::timeout, [this]() {
    //     QImage img = m_mapRenderer->render();
    //     m_mapLabel->setPixmap(QPixmap::fromImage(img));
    // });
    // m_mapTimer->start(50); // 20 FPS

    // ---- 初始化游戏 ----
    if (m_game.hasSaveFile()) {
        m_game.loadGame();
        log("📂 自动加载存档");
    } else {
        m_game.createNewCharacter("勇者");
        log("🎮 欢迎来到校园RPG冒险游戏！");
    }
    updateUI();
    updateMapUI();

    hide();
    onShowMap();
    show();
}

MainWindow::~MainWindow() {
    // delete m_mapRenderer;
    // delete m_mapTimer;
}

void MainWindow::log(const QString& msg) {
    m_log->append(msg);
}

void MainWindow::updateUI() {
    Character& p = m_game.getPlayer();
    m_nameLabel->setText(QString::fromStdString(p.getName()));
    m_levelLabel->setText(QString::number(p.getLevel()));
    m_hpBar->setMaximum(p.getMaxHp());
    m_hpBar->setValue(p.getHp());
    m_expBar->setMaximum(p.getMaxExp());
    m_expBar->setValue(p.getExp());
    m_goldLabel->setText(QString::number(p.getGold()));
    m_speedLabel->setText(QString::number(p.getSpeed()));
    m_attackLabel->setText(QString::number(p.getTotalAttack()));
    m_defenseLabel->setText(QString::number(p.getTotalDefense()));

    m_inventoryList->clear();
    m_itemCombo->clear();
    for (auto& pair : p.getInventory()) {
        QString text = QString::fromStdString(pair.first) + " x" + QString::number(pair.second.getQuantity());
        m_inventoryList->addItem(text);
        m_itemCombo->addItem(QString::fromStdString(pair.first));
    }

    m_shopCombo->clear();
    for (const auto& item : m_game.getShop().getItems()) {
        m_shopCombo->addItem(QString::fromStdString(item.getName()));
    }

    m_taskList->clear();
    for (const auto& task : m_game.getAcceptedTasks()) {
        QString text = QString::fromStdString(task.getName()) + " [" +
                       QString::fromStdString(task.getStatusString()) + "] " +
                       QString::number(task.getProgress()) + "/" + QString::number(task.getTarget());
        m_taskList->addItem(text);
    }
    for (const auto& task : m_game.getAvailableTasks()) {
        if (task.getStatus() == TaskStatus::AVAILABLE) {
            QString text = QString::fromStdString(task.getName()) + " [可接取]";
            m_taskList->addItem(text);
        }
    }

    m_quantitySpin->setValue(1);
}

void MainWindow::updateMapUI() {
    const MapArea* area = m_game.getCurrentArea();
    if (!area) return;
    m_areaLabel->setText(QString::fromStdString(area->name));
    m_areaTypeLabel->setText(QString::fromStdString(Map::getAreaTypeName(area->type)));

    m_moveCombo->clear();
    vector<string> moves = m_game.getAvailableMoves();
    for (const string& id : moves) {
        const MapArea* target = Map::getAreaById(id);
        if (target) {
            m_moveCombo->addItem(QString::fromStdString(target->name), QString::fromStdString(id));
        }
    }
    if (m_moveCombo->count() == 0) {
        m_moveCombo->addItem("没有可到达区域");
    }

    // if (m_mapRenderer) {
    //     m_mapRenderer->setCurrentArea(m_game.getCurrentAreaId());
    // }
}

// ---------- 槽函数实现 ----------
void MainWindow::onNewGame() {
    QString name = QInputDialog::getText(this, "新游戏", "请输入角色名称:");
    if (name.isEmpty()) name = "勇者";
    m_game.createNewCharacter(name.toStdString());
    updateUI();
    updateMapUI();
    log("新角色创建成功！");
}

void MainWindow::onLoadGame() {
    QString filename = QFileDialog::getOpenFileName(this, "读取存档", "./", "存档文件 (*.txt)");
    if (filename.isEmpty()) return;
    if (m_game.loadGame()) {
        updateUI();
        updateMapUI();
        log("读档成功！");
    } else {
        log("读档失败！");
    }
}

void MainWindow::onSaveGame() {
    if (m_game.saveGame()) {
        log("存档成功！");
    } else {
        log("存档失败！");
    }
}

void MainWindow::onExplore() {
    QString result = QString::fromStdString(m_game.explore());
    log(result);
    updateUI();
    updateMapUI();
}

void MainWindow::onBattle() {
    bool ok;
    QStringList items = {"史莱姆", "哥布林", "Boss"};
    QString choice = QInputDialog::getItem(this, "选择敌人", "请选择要挑战的敌人:", items, 0, false, &ok);
    if (!ok) return;

    EnemyType type;
    if (choice == "史莱姆") type = EnemyType::SLIME;
    else if (choice == "哥布林") type = EnemyType::GOBLIN;
    else type = EnemyType::BOSS;

    Enemy enemy(type);
    BattleDialog dialog(m_game.getPlayer(), enemy, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        if (!enemy.isAlive()) {
            int expGain = enemy.getExpReward();
            int goldGain = enemy.getGoldReward();
            m_game.getPlayer().addExp(expGain);
            m_game.getPlayer().addGold(goldGain);
            m_game.addStatistics("战斗胜利");
            m_game.addStatistics("获得金币", goldGain);
            m_game.addStatistics("获得经验", expGain);
            m_game.updateTaskProgress(TaskType::KILL_ENEMY);
            log(QString("🎉 战斗胜利！获得 %1 EXP, %2 金币").arg(expGain).arg(goldGain));
        } else {
            log("💀 战斗失败... 已恢复部分HP");
        }
    } else {
        log("战斗取消或逃跑");
    }
    updateUI();
    updateMapUI();
}

void MainWindow::onRest() {
    m_game.rest();
    log("休息恢复HP");
    updateUI();
}

void MainWindow::onEditCharacter() {
    EditCharacterDialog dialog(m_game.getPlayer(), this);
    if (dialog.exec() == QDialog::Accepted) {
        updateUI();
        log("角色信息已更新");
    }
}


void MainWindow::onShowMap() {
    SFMLMapWindow mapWindow(1000, 750);
    mapWindow.loadTileMap("Resources/rpg.tmx");
    mapWindow.setCurrentArea(m_game.getCurrentAreaId());
    mapWindow.run();

    std::string action = mapWindow.getActionResult();
    if (action.empty()) {
        return; // 用户直接关闭地图，停止循环
    }

    // 处理动作
    if (action == "boss") {
        onBattle();
    } else if (action == "shop") {
        ShopDialog shopDialog(m_game.getPlayer(), m_game, this);
        shopDialog.exec();
    } else if (action == "inventory") {
        InventoryDialog invDialog(m_game.getPlayer(), this);
        connect(&invDialog, &InventoryDialog::equipmentChanged, this, &MainWindow::updateUI);
        invDialog.exec();
    } else if (action == "character") {
        onEditCharacter();
    }

    // 动作处理完成后，自动重新打开地图（延迟防止事件冲突）
    QTimer::singleShot(200, this, &MainWindow::onShowMap);
}
void MainWindow::onUseItem() {
    QString itemName = m_itemCombo->currentText();
    if (itemName.isEmpty()) return;
    if (m_game.useItem(itemName.toStdString())) {
        log("使用了 " + itemName);
        updateUI();
    } else {
        log("无法使用该物品");
    }
}

void MainWindow::onDeleteItem() {
    QString itemName = m_itemCombo->currentText();
    if (itemName.isEmpty()) return;
    if (m_game.getPlayer().removeItem(itemName.toStdString())) {
        log("删除了 " + itemName);
        updateUI();
    } else {
        log("删除失败");
    }
}

void MainWindow::onBuyItem() {
    QString itemName = m_shopCombo->currentText();
    int qty = m_quantitySpin->value();
    log("尝试购买: " + itemName + " 数量: " + QString::number(qty));
    if (m_game.buyItem(itemName.toStdString(), qty)) {
        log("✅ 购买成功！");
        updateUI();
    } else {
        log("❌ 购买失败");
    }
}
void MainWindow::onSellItem() {
    QString itemName = m_shopCombo->currentText();
    int qty = m_quantitySpin->value();
    if (m_game.sellItem(itemName.toStdString(), qty)) {
        log("出售了 " + itemName + " x" + QString::number(qty));
        updateUI();
    } else {
        log("出售失败");
    }
}

void MainWindow::onAcceptTask() {
    QListWidgetItem *item = m_taskList->currentItem();
    if (!item) return;
    QString text = item->text();
    QString taskName = text.section('[', 0, 0).trimmed();
    if (m_game.acceptTask(taskName.toStdString())) {
        log("接受了任务: " + taskName);
        updateUI();
    } else {
        log("接受任务失败");
    }
}

void MainWindow::onClaimReward() {
    QListWidgetItem *item = m_taskList->currentItem();
    if (!item) return;
    QString text = item->text();
    QString taskName = text.section('[', 0, 0).trimmed();
    if (m_game.claimReward(taskName.toStdString())) {
        log("领取了奖励: " + taskName);
        updateUI();
    } else {
        log("领取奖励失败");
    }
}

void MainWindow::onShowTaskDetail() {
    QListWidgetItem *item = m_taskList->currentItem();
    if (!item) {
        log("请先选择一个任务");
        return;
    }
    QString text = item->text();
    QString taskName = text.section('[', 0, 0).trimmed();

    for (const auto& task : m_game.getAcceptedTasks()) {
        if (QString::fromStdString(task.getName()) == taskName) {
            TaskDetailDialog dialog(task, this);
            dialog.exec();
            return;
        }
    }
    for (const auto& task : m_game.getAvailableTasks()) {
        if (QString::fromStdString(task.getName()) == taskName) {
            TaskDetailDialog dialog(task, this);
            dialog.exec();
            return;
        }
    }
    log("未找到任务详情");
}

void MainWindow::onMove() {
    int index = m_moveCombo->currentIndex();
    if (index < 0) return;
    QString areaId = m_moveCombo->itemData(index).toString();
    if (areaId.isEmpty()) return;
    if (m_game.moveTo(areaId.toStdString())) {
        const MapArea* area = m_game.getCurrentArea();
        log("移动到了 " + QString::fromStdString(area->name));
        updateMapUI();
    } else {
        log("移动失败！");
    }
}