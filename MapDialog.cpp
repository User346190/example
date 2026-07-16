#include "MapDialog.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QPixmap>
#include <QCloseEvent>

MapDialog::MapDialog(SFMLMapRenderer* renderer, QWidget* parent)
    : QDialog(parent), m_renderer(renderer) {
    setWindowTitle("🗺️ 校园地图");
    resize(820, 640);

    m_mapLabel = new QLabel(this);
    m_mapLabel->setFixedSize(800, 600);
    m_mapLabel->setScaledContents(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_mapLabel);

    // 定时刷新
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [this]() {
        if (m_renderer) {
            try {
                QImage img = m_renderer->render();
                m_mapLabel->setPixmap(QPixmap::fromImage(img));
            } catch (...) {
                // 忽略渲染异常，避免崩溃
            }
        }
    });
    m_timer->start(50);
}

MapDialog::~MapDialog() {
    m_timer->stop();
}

void MapDialog::closeEvent(QCloseEvent* event) {
    m_timer->stop();
    event->accept();
}