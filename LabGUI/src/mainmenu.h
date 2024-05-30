#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "submenu.h"

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    void widgetSelected(QWidget *widget); // Signal emitted when a widget is selected

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void toggleSubMenu(int index);
    void clearButtonSelection(int indexToSkip);

private:
    QVBoxLayout *layout;
    QList<QPushButton*> mainButtons;
    QList<SubMenu*> subMenus;
    SubMenu *currentMenu;
    QPropertyAnimation *expandAnimation;
    QPropertyAnimation *collapseAnimation;
    const int collapsedWidth = 60;
    const int expandedWidth = 200;
};

#endif // MAINMENU_H
