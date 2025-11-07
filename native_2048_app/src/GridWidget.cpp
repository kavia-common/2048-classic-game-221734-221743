#include "GridWidget.h"
#include "TileWidget.h"
#include "Theme.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>

static constexpr int SIZE = 4;

GridWidget::GridWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(360, 360);
    setAutoFillBackground(false);

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(Theme::shadowBlurRadius());
    shadow->setColor(Theme::shadowColor());
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);

    ensureTiles();
}

void GridWidget::ensureTiles() {
    if (!m_tiles.isEmpty()) return;
    m_tiles.resize(SIZE);
    for (int r=0;r<SIZE;++r) {
        m_tiles[r].resize(SIZE);
        for (int c=0;c<SIZE;++c) {
            auto* t = new TileWidget(this);
            t->setValue(0);
            m_tiles[r][c] = t;
            t->show();
        }
    }
}

QRect GridWidget::tileRect(int row, int col) const {
    int spacing = Theme::tileSpacing();
    int w = width();
    int h = height();
    int gridSize = qMin(w, h);
    int tileSize = (gridSize - spacing * (SIZE + 1)) / SIZE;
    int startX = (w - gridSize)/2 + spacing;
    int startY = (h - gridSize)/2 + spacing;
    int x = startX + col * (tileSize + spacing);
    int y = startY + row * (tileSize + spacing);
    return QRect(x, y, tileSize, tileSize);
}

void GridWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    layoutTiles(false);
}

void GridWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background rounded board
    QRectF r = rect().adjusted(4,4,-4,-4);
    QPainterPath path;
    path.addRoundedRect(r, Theme::cornerRadius(), Theme::cornerRadius());
    p.fillPath(path, Theme::surface());

    // Draw grid cells background
    for (int rIdx=0; rIdx<SIZE; ++rIdx) {
        for (int cIdx=0; cIdx<SIZE; ++cIdx) {
            QRect cell = tileRect(rIdx, cIdx);
            QPainterPath tilePath;
            tilePath.addRoundedRect(cell, Theme::tileCornerRadius(), Theme::tileCornerRadius());
            p.fillPath(tilePath, Theme::gridBackground());
        }
    }
}

void GridWidget::layoutTiles(bool animate) {
    ensureTiles();
    QParallelAnimationGroup* group = nullptr;
    if (animate) group = new QParallelAnimationGroup(this);

    for (int r=0;r<SIZE;++r) {
        for (int c=0;c<SIZE;++c) {
            TileWidget* t = m_tiles[r][c];
            QRect target = tileRect(r,c);
            if (animate) {
                auto* anim = new QPropertyAnimation(t, "geometry");
                anim->setDuration(Theme::animationDurationMs());
                anim->setStartValue(t->geometry());
                anim->setEndValue(target);
                anim->setEasingCurve(QEasingCurve::OutCubic);
                group->addAnimation(anim);
            } else {
                t->setGeometry(target);
            }
        }
    }
    if (group) {
        QObject::connect(group, &QParallelAnimationGroup::finished, group, &QObject::deleteLater);
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void GridWidget::setBoard(const QVector<QVector<int>>& board, bool animate) {
    m_board = board;
    ensureTiles();
    for (int r=0;r<SIZE;++r) {
        for (int c=0;c<SIZE;++c) {
            m_tiles[r][c]->setValue(m_board[r][c], true);
        }
    }
    layoutTiles(animate);
    update();
}
