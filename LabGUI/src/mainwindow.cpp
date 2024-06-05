#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    

    menu = new MainMenu(this);
    placeholderWidget = new BaseInterfaceWidget(this);
    // interfacehuj = new interface(this);
    mainLayout->addWidget(menu);
    mainLayout->addWidget(placeholderWidget);

    menu->stackUnder(placeholderWidget);
    
    QPixmap backgroundImage("../res/WSiMR.png"); // Load image from resources
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    placeholderWidget->setAutoFillBackground(true);
    placeholderWidget->setPalette(palette);
    placeholderWidget->setBackgroundRole(QPalette::Window);
    placeholderWidget->setStyleSheet("background-image: url(../res/WSiMR.png); background-attachment: fixed; background-repeat: no-repeat; background-position: center;");

    setCentralWidget(centralWidget);
    // setCentralWidget(interfacehuj);

    connect(menu, &MainMenu::widgetSelected, this, &MainWindow::changeInterfaceWidget);
    resize(800, 600);
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


void MainWindow::changeInterfaceWidget(QWidget *widget) {
    QLayoutItem *child;
    while ((child = placeholderWidget->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (widget != nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(placeholderWidget);
        layout->addWidget(widget);
        placeholderWidget->setLayout(layout);
        placeholderWidget->show();
    } else {
        placeholderWidget->hide();
    }
}
