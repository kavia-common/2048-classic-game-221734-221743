#include "TileWidget.h"
#include "Theme.h"
#include <QPainter>
#include <QEasingCurve>

TileWidget::TileWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_anim = new QPropertyAnimation(this, "scale");
    m_anim->setStartValue(0.9);
    m_anim->setEndValue(1.0);
    m_anim->setDuration(Theme::animationDurationMs());
    m_anim->setEasingCurve(QEasingCurve::OutBack);
}

void TileWidget::setValue(int value, bool animate) {
    bool changed = (m_value != value);
    m_value = value;
    if (animate && changed && m_value != 0) {
        startPopAnimation(Theme::animationDurationMs());
    }
    update();
}

int TileWidget::value() const {
    return m_value;
}

void TileWidget::setScale(qreal s) {
    m_scale = s;
    update();
}

void TileWidget::startPopAnimation(int durationMs) {
    m_anim->stop();
    m_anim->setDuration(durationMs);
    m_anim->setStartValue(0.8);
    m_anim->setEndValue(1.0);
    m_anim->start();
}

void TileWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRectF r = rect();
    // Apply scale transform around center
    QPointF center = r.center();
    p.translate(center);
    p.scale(m_scale, m_scale);
    p.translate(-center);

    // Background
    QColor bg = Theme::tileBackground(m_value);
    QColor fg = Theme::tileText(m_value);

    QPainterPath path;
    path.addRoundedRect(r.adjusted(1,1,-1,-1), Theme::tileCornerRadius(), Theme::tileCornerRadius());
    p.fillPath(path, bg);

    if (m_value > 0) {
        p.setPen(fg);
        QFont f = Theme::tileFont(m_value);
        p.setFont(f);
        QString text = QString::number(m_value);
        p.drawText(r, Qt::AlignCenter, text);
    }
}
