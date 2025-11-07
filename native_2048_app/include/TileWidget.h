#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>

/**
 * PUBLIC_INTERFACE
 * TileWidget renders a tile value with rounded background and animated scale.
 */
class TileWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    explicit TileWidget(QWidget* parent = nullptr);

    // PUBLIC_INTERFACE
    void setValue(int value, bool animate = false);
    int value() const;

    qreal scale() const { return m_scale; }
    void setScale(qreal s);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int m_value = 0;
    qreal m_scale = 1.0;
    QPropertyAnimation* m_anim = nullptr;

    void startPopAnimation(int durationMs);
};

#endif // TILEWIDGET_H
