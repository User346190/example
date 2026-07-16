#include "EditCharacterDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

EditCharacterDialog::EditCharacterDialog(Character& character, QWidget *parent)
    : QDialog(parent), m_character(character) {
    setWindowTitle("编辑角色");
    setFixedSize(300, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("角色名:"));
    m_nameEdit = new QLineEdit(QString::fromStdString(m_character.getName()));
    layout->addWidget(m_nameEdit);

    layout->addWidget(new QLabel("等级:"));
    m_levelSpin = new QSpinBox;
    m_levelSpin->setRange(1, 100);
    m_levelSpin->setValue(m_character.getLevel());
    layout->addWidget(m_levelSpin);

    layout->addWidget(new QLabel("HP:"));
    m_hpSpin = new QSpinBox;
    m_hpSpin->setRange(1, 9999);
    m_hpSpin->setValue(m_character.getHp());
    layout->addWidget(m_hpSpin);

    layout->addWidget(new QLabel("攻击力:"));
    m_attackSpin = new QSpinBox;
    m_attackSpin->setRange(1, 999);
    m_attackSpin->setValue(m_character.getAttack());
    layout->addWidget(m_attackSpin);

    layout->addWidget(new QLabel("防御力:"));
    m_defenseSpin = new QSpinBox;
    m_defenseSpin->setRange(0, 999);
    m_defenseSpin->setValue(m_character.getDefense());
    layout->addWidget(m_defenseSpin);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *btnSave = new QPushButton("保存");
    QPushButton *btnCancel = new QPushButton("取消");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &EditCharacterDialog::onSave);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void EditCharacterDialog::onSave() {
    m_character.setName(m_nameEdit->text().toStdString());
    m_character.setLevel(m_levelSpin->value());
    m_character.setHp(m_hpSpin->value());
    m_character.setAttack(m_attackSpin->value());
    m_character.setDefense(m_defenseSpin->value());
    accept();
}