#include "TaskDetailDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

TaskDetailDialog::TaskDetailDialog(const Task& task, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("📜 任务详情");
    setFixedSize(400, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel(QString::fromStdString(task.getName()));
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffd700;");
    layout->addWidget(titleLabel);

    layout->addWidget(new QLabel("类型: " + QString::fromStdString(task.getTypeString())));
    layout->addWidget(new QLabel("状态: " + QString::fromStdString(task.getStatusString())));

    QLabel *descLabel = new QLabel(QString::fromStdString(task.getDescription()));
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    // 进度条
    QProgressBar *progressBar = new QProgressBar;
    progressBar->setRange(0, task.getTarget());
    progressBar->setValue(task.getProgress());
    progressBar->setFormat(QString::number(task.getProgress()) + "/" + QString::number(task.getTarget()));
    layout->addWidget(progressBar);

    layout->addWidget(new QLabel("奖励: " +
        QString::number(task.getExpReward()) + " EXP, " +
        QString::number(task.getGoldReward()) + " 金币"));

    if (!task.getItemRewards().empty()) {
        QString items;
        for (const auto& item : task.getItemRewards()) {
            items += QString::fromStdString(item.getName()) + " ";
        }
        layout->addWidget(new QLabel("物品奖励: " + items));
    }

    QPushButton *btnClose = new QPushButton("关闭");
    layout->addWidget(btnClose);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
}