#include "BattleDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QTimer>

BattleDialog::BattleDialog(Character& player, Enemy& enemy, QWidget *parent)
    : QDialog(parent), m_player(player), m_enemy(enemy),
      m_playerDefending(false), m_battleEnded(false) {
    setWindowTitle("⚔️ 战斗");
    setFixedSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ---- 敌我信息 ----
    QHBoxLayout *infoLayout = new QHBoxLayout;

    // 玩家信息
    QGroupBox *playerBox = new QGroupBox("👤 玩家");
    QVBoxLayout *playerLayout = new QVBoxLayout(playerBox);
    m_playerNameLabel = new QLabel(QString::fromStdString(m_player.getName()));
    playerLayout->addWidget(m_playerNameLabel);
    m_playerHpBar = new QProgressBar;
    m_playerHpBar->setRange(0, m_player.getMaxHp());
    m_playerHpBar->setValue(m_player.getHp());
    m_playerHpBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");
    playerLayout->addWidget(m_playerHpBar);
    m_playerHpText = new QLabel(QString::number(m_player.getHp()) + "/" + QString::number(m_player.getMaxHp()));
    playerLayout->addWidget(m_playerHpText);
    infoLayout->addWidget(playerBox);

    // VS
    QLabel *vsLabel = new QLabel("⚔️ VS");
    vsLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #ff6b35;");
    infoLayout->addWidget(vsLabel);

    // 敌人信息
    QGroupBox *enemyBox = new QGroupBox("👾 敌人");
    QVBoxLayout *enemyLayout = new QVBoxLayout(enemyBox);
    m_enemyNameLabel = new QLabel(QString::fromStdString(m_enemy.getName()));
    enemyLayout->addWidget(m_enemyNameLabel);
    m_enemyHpBar = new QProgressBar;
    m_enemyHpBar->setRange(0, m_enemy.getMaxHp());
    m_enemyHpBar->setValue(m_enemy.getHp());
    m_enemyHpBar->setStyleSheet("QProgressBar::chunk { background-color: #f44336; }");
    enemyLayout->addWidget(m_enemyHpBar);
    m_enemyHpText = new QLabel(QString::number(m_enemy.getHp()) + "/" + QString::number(m_enemy.getMaxHp()));
    enemyLayout->addWidget(m_enemyHpText);
    infoLayout->addWidget(enemyBox);

    mainLayout->addLayout(infoLayout);

    // ---- 日志 ----
    m_log = new QTextEdit;
    m_log->setReadOnly(true);
    m_log->setMaximumHeight(150);
    m_log->setStyleSheet("background-color: rgba(0,0,0,0.3); color: #c8d6e5; border-radius: 10px; padding: 5px;");
    mainLayout->addWidget(m_log);

    // ---- 操作按钮 ----
    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_btnAttack = new QPushButton("⚔️ 攻击");
    m_btnDefend = new QPushButton("🛡️ 防御");
    m_btnUseItem = new QPushButton("🧪 使用物品");
    m_btnFlee = new QPushButton("🏃 逃跑");
    btnLayout->addWidget(m_btnAttack);
    btnLayout->addWidget(m_btnDefend);
    btnLayout->addWidget(m_btnUseItem);
    btnLayout->addWidget(m_btnFlee);
    mainLayout->addLayout(btnLayout);

    connect(m_btnAttack, &QPushButton::clicked, this, &BattleDialog::onAttack);
    connect(m_btnDefend, &QPushButton::clicked, this, &BattleDialog::onDefend);
    connect(m_btnUseItem, &QPushButton::clicked, this, &BattleDialog::onUseItem);
    connect(m_btnFlee, &QPushButton::clicked, this, &BattleDialog::onFlee);

    addLog("⚔️ 战斗开始！");
    addLog(QString::fromStdString("你遭遇了 " + m_enemy.getName()) + "！");
    updateUI();
}

void BattleDialog::addLog(const QString& msg) {
    m_log->append(msg);
}

void BattleDialog::updateUI() {
    m_playerHpBar->setValue(m_player.getHp());
    m_playerHpText->setText(QString::number(m_player.getHp()) + "/" + QString::number(m_player.getMaxHp()));
    m_enemyHpBar->setValue(m_enemy.getHp());
    m_enemyHpText->setText(QString::number(m_enemy.getHp()) + "/" + QString::number(m_enemy.getMaxHp()));

    // 更新进度条颜色
    if (m_player.getHp() < m_player.getMaxHp() * 0.3) {
        m_playerHpBar->setStyleSheet("QProgressBar::chunk { background-color: #f44336; }");
    } else if (m_player.getHp() < m_player.getMaxHp() * 0.6) {
        m_playerHpBar->setStyleSheet("QProgressBar::chunk { background-color: #ff9800; }");
    } else {
        m_playerHpBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");
    }
}

void BattleDialog::onAttack() {
    if (m_battleEnded) return;
    // 玩家攻击
    int damage = std::max(1, m_player.getAttack() - m_enemy.getDefense() / 2 + (rand() % 9 - 4));
    m_enemy.takeDamage(damage);
    addLog(QString("💥 你对 %1 造成 %2 点伤害！")
           .arg(QString::fromStdString(m_enemy.getName()))
           .arg(damage));
    updateUI();

    if (!m_enemy.isAlive()) {
        endBattle(true);
        return;
    }

    enemyTurn();
}

void BattleDialog::onDefend() {
    if (m_battleEnded) return;
    m_playerDefending = true;
    addLog("🛡️ 你采取了防御姿态！");
    enemyTurn();
}

void BattleDialog::onUseItem() {
    if (m_battleEnded) return;
    // 简单使用物品：恢复HP
    if (m_player.getHp() < m_player.getMaxHp()) {
        int heal = 30 + m_player.getLevel() * 5;
        m_player.addHp(heal);
        addLog(QString("🧪 使用物品，恢复 %1 HP").arg(heal));
        updateUI();
    } else {
        addLog("❌ HP已满，不需要使用物品");
    }
    enemyTurn();
}

void BattleDialog::onFlee() {
    if (m_battleEnded) return;
    int chance = rand() % 100;
    if (chance > 40) {
        addLog("🏃 成功逃跑！");
        m_battleEnded = true;
        emit battleFinished(false);
        reject();
    } else {
        addLog("❌ 逃跑失败！");
        enemyTurn();
    }
}

void BattleDialog::enemyTurn() {
    if (m_battleEnded) return;
    int damage = std::max(1, m_enemy.getAttack() - m_player.getDefense() / 2 + (rand() % 9 - 4));
    if (m_playerDefending) {
        damage = damage / 2;
        m_playerDefending = false;
        addLog("🛡️ 防御姿态减少了一半伤害！");
    }
    m_player.addHp(-damage);
    addLog(QString("💢 %1 对你造成 %2 点伤害！")
           .arg(QString::fromStdString(m_enemy.getName()))
           .arg(damage));
    updateUI();

    if (!m_player.isAlive()) {
        endBattle(false);
    }
}

void BattleDialog::endBattle(bool won) {
    m_battleEnded = true;
    if (won) {
        addLog("🎉 战斗胜利！");
        // 奖励在外部处理
    } else {
        addLog("💀 你被击败了...");
    }
    emit battleFinished(won);

    // 禁用按钮
    m_btnAttack->setEnabled(false);
    m_btnDefend->setEnabled(false);
    m_btnUseItem->setEnabled(false);
    m_btnFlee->setEnabled(false);

    // 延迟关闭
    QTimer::singleShot(2000, this, &QDialog::accept);
}