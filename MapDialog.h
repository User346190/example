#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>
#include <QLabel>
#include "SFMLMapRenderer.h"

class MapDialog : public QDialog {
    Q_OBJECT
public:
    explicit MapDialog(SFMLMapRenderer* renderer, QWidget* parent = nullptr);
    ~MapDialog();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QLabel* m_mapLabel;
    SFMLMapRenderer* m_renderer;
    QTimer* m_timer;
};

#endif