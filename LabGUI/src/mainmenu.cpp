#include "mainmenu.h"
#include <QEnterEvent>
#include <QGraphicsDropShadowEffect>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent), currentMenu(nullptr) {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addStretch(1);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setOffset(0, 0);
    setGraphicsEffect(shadowEffect);

    
    for (int i = 0; i < 3; ++i) {
        QPushButton *button = new QPushButton(menuTitles[i], this);
        
        button->setMinimumHeight(30);
        button->setFlat(true);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton { border: none; background-color: transparent; }"
                              "QPushButton:pressed { background-color: transparent; }"
                              "QPushButton:checked { background-color: #003965; }");

        layout->insertWidget(layout->count() - 1, button); // Insert before the stretch
        mainButtons.append(button);

        SubMenu *subMenu = new SubMenu(i, this);
        layout->insertWidget(layout->count() - 1, subMenu); // Insert before the stretch
        subMenus.append(subMenu);

        subMenu->setVisible(false); // Initially hide all submenus

        // Connect the button click to toggle the corresponding submenu
        connect(button, &QPushButton::clicked, this, [this, i]() {
            toggleSubMenu(i);
            // Uncheck other buttons
            clearButtonSelection(i);
        });

        // Connect SubMenu's widgetSelected signal to MainMenu's widgetSelected signal
        connect(subMenu, &SubMenu::buttonClicked, this, &MainMenu::widgetSelected);
    }

    setLayout(layout);
    setFixedWidth(collapsedWidth);

    expandAnimation = new QPropertyAnimation(this, "minimumWidth");
    expandAnimation->setDuration(100);
    expandAnimation->setStartValue(collapsedWidth);
    expandAnimation->setEndValue(expandedWidth);

    collapseAnimation = new QPropertyAnimation(this, "minimumWidth");
    collapseAnimation->setDuration(100);
    collapseAnimation->setStartValue(expandedWidth);
    collapseAnimation->setEndValue(collapsedWidth);
}

void MainMenu::enterEvent(QEnterEvent *event) {
    expandAnimation->start();
    if (currentMenu)
        currentMenu->toggleSubMenu();
    QWidget::enterEvent(event);
}

void MainMenu::leaveEvent(QEvent *event) {
    collapseAnimation->start();
    if (currentMenu)
        currentMenu->collapseSubMenu();
    QWidget::leaveEvent(event);
}

void MainMenu::toggleSubMenu(int index) {
    if (currentMenu && currentMenu != subMenus[index]) {
        currentMenu->collapseSubMenu();
        currentMenu->clearButtonSelection(-1);
    }
    currentMenu = subMenus[index]->expanded ? nullptr : subMenus[index];
    
    subMenus[index]->toggleSubMenu();
}

void MainMenu::clearButtonSelection(int indexToSkip) {
    for (int j = 0; j < mainButtons.size(); ++j) {
        if (j != indexToSkip) {
            mainButtons[j]->setChecked(false);
        }
    }
}