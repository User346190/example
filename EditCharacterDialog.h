#ifndef EDITCHARACTERDIALOG_H
#define EDITCHARACTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "Character.h"

class EditCharacterDialog : public QDialog {
    Q_OBJECT
public:
    EditCharacterDialog(Character& character, QWidget *parent = nullptr);

private slots:
    void onSave();

private:
    Character& m_character;
    QLineEdit *m_nameEdit;
    QSpinBox *m_levelSpin;
    QSpinBox *m_hpSpin;
    QSpinBox *m_attackSpin;
    QSpinBox *m_defenseSpin;
};

#endif