#ifndef SUBMENU_H
#define SUBMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>

class SubMenu : public QWidget {
    Q_OBJECT

public:
    explicit SubMenu(QWidget *parent = nullptr);

    void toggleSubMenu();
    void collapseSubMenu();

    bool expanded;

signals:
    void widgetSelected(QWidget *widget);
    
private:
    QVBoxLayout *layout;
    QList<QPushButton*> subButtons;
    QPropertyAnimation *expandAnimation;
    QPropertyAnimation *collapseAnimation;
    static constexpr int collapsedHeight = 0;
    static constexpr int expandedHeight = 100; // Adjust this as needed
};

#endif // SUBMENU_H
