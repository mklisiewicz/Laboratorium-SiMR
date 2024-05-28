#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    createMenu();

    // Create an instance of ImageWidget and set it as the central widget
    interfaceWidget = new interface(this);
    setCentralWidget(interfaceWidget);
}

void MainWindow::createMenu() {
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("File", this);
    QAction *exitAction = new QAction("Exit", this);

    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    QMenu *helpMenu = new QMenu("Help", this);
    QAction *aboutAction = new QAction("About", this);

    connect(aboutAction, &QAction::triggered, this, &MainWindow::onActionTriggered);

    helpMenu->addAction(aboutAction);
    menuBar->addMenu(helpMenu);

    setMenuBar(menuBar);
}

void MainWindow::onActionTriggered() {
    QLabel *aboutLabel = new QLabel("This is a simple Qt application with a menu and a frame.");
    aboutLabel->setWindowFlag(Qt::Window);
    aboutLabel->show();
}


