#include "Theme.h"
#include <QFont>

QColor Theme::primary() { return QColor("#2563EB"); }
QColor Theme::secondary() { return QColor("#F59E0B"); }
QColor Theme::success() { return QColor("#F59E0B"); }
QColor Theme::error() { return QColor("#EF4444"); }
QColor Theme::background() { return QColor("#f9fafb"); }
QColor Theme::surface() { return QColor("#ffffff"); }
QColor Theme::text() { return QColor("#111827"); }
QColor Theme::gridBackground() { return QColor("#dce3ef"); }
QColor Theme::shadowColor() { return QColor(0, 0, 0, 50); }

QColor Theme::tileBackground(int value) {
    // Inspired by Ocean Professional palette
    switch (value) {
        case 0: return QColor(255, 255, 255, 0);
        case 2: return QColor("#e6f0ff");
        case 4: return QColor("#d6e6ff");
        case 8: return QColor("#c7dcff");
        case 16: return QColor("#b8d2ff");
        case 32: return QColor("#a9c8ff");
        case 64: return QColor("#8fb6ff");
        case 128: return QColor("#fef3c7"); // amber-100
        case 256: return QColor("#fde68a"); // amber-300
        case 512: return QColor("#f59e0b"); // amber-500
        case 1024: return QColor("#ef4444"); // red-500
        case 2048: return QColor("#10b981"); // green-500 (win)
        default: return QColor("#374151"); // gray-700 for larger tiles
    }
}

QColor Theme::tileText(int value) {
    if (value <= 4) return QColor("#111827");
    return QColor("#ffffff");
}

int Theme::cornerRadius() { return 12; }
int Theme::tileCornerRadius() { return 10; }
int Theme::tileSpacing() { return 10; }
int Theme::headerHeight() { return 64; }
int Theme::animationDurationMs() { return 180; }
int Theme::shadowBlurRadius() { return 18; }

QFont Theme::titleFont() {
    QFont f; f.setPointSize(24); f.setBold(true); return f;
}
QFont Theme::scoreFont() {
    QFont f; f.setPointSize(12); f.setBold(true); return f;
}
QFont Theme::tileFont(int value) {
    QFont f;
    if (value < 100) f.setPointSize(22);
    else if (value < 1000) f.setPointSize(20);
    else f.setPointSize(18);
    f.setBold(true);
    return f;
}
