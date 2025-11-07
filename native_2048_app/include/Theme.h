#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QFont>
#include <QMap>

/**
 * PUBLIC_INTERFACE
 * Theme provides centralized colors, metrics, and fonts for the app.
 */
class Theme {
public:
    // Colors
    static QColor primary();     // #2563EB
    static QColor secondary();   // #F59E0B
    static QColor success();     // #F59E0B
    static QColor error();       // #EF4444
    static QColor background();  // #f9fafb
    static QColor surface();     // #ffffff
    static QColor text();        // #111827
    static QColor gridBackground();
    static QColor tileBackground(int value);
    static QColor tileText(int value);
    static QColor shadowColor();

    // Metrics
    static int cornerRadius();           // Rounded corners radius
    static int tileCornerRadius();
    static int tileSpacing();
    static int headerHeight();
    static int animationDurationMs();    // 150-250ms
    static int shadowBlurRadius();

    // Fonts
    static QFont titleFont();
    static QFont scoreFont();
    static QFont tileFont(int value);
};

#endif // THEME_H
