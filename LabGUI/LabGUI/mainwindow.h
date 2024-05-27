#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include "interface.h"


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
    void onActionTriggered();

private:
    void createMenu();
    QFrame *centralFrame;
    interface *interfaceWidget;
};
#endif // MAINWINDOW_H
