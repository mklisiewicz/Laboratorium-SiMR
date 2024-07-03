#ifndef SUBMENU_H
#define SUBMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>

class SubMenu : public QWidget {
    Q_OBJECT

public:
    explicit SubMenu(int menuIndex, QWidget *parent = nullptr);
    void clearButtonSelection(int indexToSkip);
    void toggleSubMenu();
    void collapseSubMenu();

    bool expanded;

signals:
    void widgetSelected(QWidget *widget);
    void buttonClicked(int buttonIndex);
private:
    QVBoxLayout *layout;
    QList<QPushButton*> subButtons;
    QPropertyAnimation *expandAnimation;
    QPropertyAnimation *collapseAnimation;
    static constexpr int collapsedHeight = 0;
    static constexpr int expandedHeight = 100; // Adjust this as needed
    QList<QString> drganiaTitles = {"Płaszczyzna Fazowa",
                                    "Dynamiczny Eliminator Drgań",
                                    "Układ o Dwóch Stopniach Swobody",
                                    "Częstotliwość Drgań Własnych Pręta",
                                    "Układ Liniowy"};
    QList<QString> pkmTitles = {"Płaszczyzna Fazowa",
                                    "Dynamiczny Eliminator Drgań",
                                    "Układ o Dwóch Stopniach Swobody",
                                    "Częstotliwość Drgań Własnych Pręta",
                                    "Układ Liniowy"};
    QList<QString> fluidTitles = {"Płaszczyzna Fazowa",
                                    "Dynamiczny Eliminator Drgań",
                                    "Układ o Dwóch Stopniach Swobody",
                                    "Częstotliwość Drgań Własnych Pręta",
                                    "Układ Liniowy"};
    QList<QList<QString>*> subtitles = {&drganiaTitles,
                                        &pkmTitles,
                                        &fluidTitles};
};

#endif // SUBMENU_H
