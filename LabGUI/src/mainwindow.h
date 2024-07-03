#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFrame>
#include <QPalette>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QDockWidget>
#include "interface.h"
#include "mainmenu.h"
#include "baseinterfacewidget.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createMenu();
    void onActionTriggered();
    void changeInterfaceWidget(int widgetIndex);

private:
    MainMenu *menu;
    BaseInterfaceWidget *interfaceWidget;
    QWidget *placeholderWidget;
    interface *interfacehuj;
};
#endif // MAINWINDOW_H
