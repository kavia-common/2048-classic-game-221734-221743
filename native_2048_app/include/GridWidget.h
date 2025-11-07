#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QVector>

class TileWidget;

/**
 * PUBLIC_INTERFACE
 * GridWidget draws the board background and arranges 4x4 TileWidgets with simple slide animations.
 */
class GridWidget : public QWidget {
    Q_OBJECT
public:
    explicit GridWidget(QWidget* parent = nullptr);

    // PUBLIC_INTERFACE
    void setBoard(const QVector<QVector<int>>& board, bool animate = true);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QVector<int>> m_board;
    QVector<QVector<TileWidget*>> m_tiles;

    QRect tileRect(int row, int col) const;
    void layoutTiles(bool animate);
    void ensureTiles();
};

#endif // GRIDWIDGET_H
