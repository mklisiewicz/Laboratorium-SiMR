#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

// void setDarkMode(QWidget *widget, bool darkMode) {
//     if (darkMode) {
//         qApp->setStyleSheet("QMainWindow { background-color: #333; color: #fff; }");
//         widget->setStyleSheet("background-color: #666; color: #fff;");
//     } else {
//         qApp->setStyleSheet("");
//         widget->setStyleSheet("");
//     }
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("../res/WSiMR.ico"));

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LabGUI_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    // QPalette palette;
    // palette.setColor(QPalette::Window, Qt::black); // Set window background color to white
    // palette.setColor(QPalette::WindowText, Qt::white); // Set text color to black
    // a.setPalette(palette);
    // setDarkMode(&w, true);
    w.show();
    return a.exec();
}

