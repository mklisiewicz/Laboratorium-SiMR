#include "submenu.h"

SubMenu::SubMenu(QWidget *parent) : QWidget(parent), expanded(false) {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create sub-buttons
    for (int i = 0; i < 3; ++i) { // Adjust the number of sub-buttons as needed
        QPushButton *button = new QPushButton(QString("Sub Button %1").arg(i + 1), this);
        
        button->setMinimumHeight(20);
        button->setFlat(true);
        button->setStyleSheet("QPushButton { border: none; background-color: transparent; }"
                              "QPushButton:pressed { background-color: lightgray; }");

        layout->addWidget(button);
        subButtons.append(button);
    }

    setLayout(layout);

    expandAnimation = new QPropertyAnimation(this, "maximumHeight");
    expandAnimation->setDuration(100);

    collapseAnimation = new QPropertyAnimation(this, "maximumHeight");
    collapseAnimation->setDuration(100);

    setMaximumHeight(collapsedHeight); // Initially collapsed
    setVisible(false);
}

void SubMenu::toggleSubMenu() {
    int contentHeight = layout->sizeHint().height();

    if (expanded) {
        collapseAnimation->setStartValue(contentHeight);
        collapseAnimation->setEndValue(collapsedHeight);
        collapseAnimation->start();
        connect(collapseAnimation, &QPropertyAnimation::finished, this, &QWidget::hide);
    } else {
        setVisible(true);
        expandAnimation->setStartValue(collapsedHeight);
        expandAnimation->setEndValue(contentHeight);
        expandAnimation->start();
    }
    expanded = !expanded;
}

void SubMenu::collapseSubMenu() {
    if (expanded) {
        int contentHeight = layout->sizeHint().height();
        collapseAnimation->setStartValue(contentHeight);
        collapseAnimation->setEndValue(collapsedHeight);
        collapseAnimation->start();
        connect(collapseAnimation, &QPropertyAnimation::finished, this, &QWidget::hide);
        expanded = false;
    }
}
