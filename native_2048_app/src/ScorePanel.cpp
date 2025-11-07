#include "ScorePanel.h"
#include "Theme.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

ScorePanel::ScorePanel(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(12);

    auto tagStyle = QString("QLabel{background:%1;color:%2;border-radius:%3px;padding:8px 12px;font-weight:bold;}")
            .arg(Theme::primary().name())
            .arg("#ffffff")
            .arg(Theme::cornerRadius()-4);

    auto valueStyle = QString("QLabel{background:%1;color:%2;border-radius:%3px;padding:6px 10px;font-weight:bold;}")
            .arg(Theme::surface().name())
            .arg(Theme::text().name())
            .arg(Theme::cornerRadius()-6);

    auto* scoreLabel = new QLabel("SCORE", this);
    scoreLabel->setStyleSheet(tagStyle);
    m_scoreValue = new QLabel("0", this);
    m_scoreValue->setStyleSheet(valueStyle);
    m_scoreValue->setFont(Theme::scoreFont());

    auto* bestLabel = new QLabel("BEST", this);
    bestLabel->setStyleSheet(tagStyle);
    m_bestValue = new QLabel("0", this);
    m_bestValue->setStyleSheet(valueStyle);
    m_bestValue->setFont(Theme::scoreFont());

    layout->addWidget(scoreLabel);
    layout->addWidget(m_scoreValue);
    layout->addSpacing(8);
    layout->addWidget(bestLabel);
    layout->addWidget(m_bestValue);
    layout->addStretch(1);

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(Theme::shadowBlurRadius());
    shadow->setColor(Theme::shadowColor());
    shadow->setOffset(0, 2);
    setGraphicsEffect(shadow);
}

void ScorePanel::setScores(int score, int best) {
    m_scoreValue->setText(QString::number(score));
    m_bestValue->setText(QString::number(best));
}
