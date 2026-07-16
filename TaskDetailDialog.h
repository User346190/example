#ifndef TASKDETAILDIALOG_H
#define TASKDETAILDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include "Task.h"

class TaskDetailDialog : public QDialog {
    Q_OBJECT
public:
    TaskDetailDialog(const Task& task, QWidget *parent = nullptr);
};

#endif