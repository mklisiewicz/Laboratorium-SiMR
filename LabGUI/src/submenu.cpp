#include "submenu.h"

SubMenu::SubMenu(int menuIndex, QWidget *parent) : QWidget(parent), expanded(false) {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QList<QString> *titles = subtitles[menuIndex];
    
    for (int i = 0; i < titles->size(); ++i) { // Adjust the number of sub-buttons as needed
        QPushButton *button = new QPushButton(titles->at(i), this);
        
        button->setMinimumHeight(20);
        button->setFlat(true);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton { border: none; background-color: transparent; }"
                              "QPushButton:pressed { background-color: transparent; }"
                              "QPushButton:checked { background-color: #003965; }");
        
        layout->addWidget(button);
        subButtons.append(button);

        connect(button, &QPushButton::clicked, this, [this, i, menuIndex]() {
            clearButtonSelection(i);
            emit buttonClicked(menuIndex*10+i);
        });
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

void SubMenu::clearButtonSelection(int indexToSkip) {
    for (int j = 0; j < subButtons.size(); ++j) {
        if (j != indexToSkip) {
            subButtons[j]->setChecked(false);
        }
    }
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
