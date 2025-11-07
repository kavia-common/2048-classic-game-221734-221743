#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

class QLabel;

/**
 * PUBLIC_INTERFACE
 * ScorePanel renders current and best score with Ocean styling.
 */
class ScorePanel : public QWidget {
    Q_OBJECT
public:
    explicit ScorePanel(QWidget* parent = nullptr);

public slots:
    // PUBLIC_INTERFACE
    void setScores(int score, int best);

private:
    QLabel* m_scoreValue;
    QLabel* m_bestValue;
};

#endif // SCOREPANEL_H
