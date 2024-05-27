#ifndef INTERFACE_H
#define INTERFACE_H
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

#include <QWidget>
#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QCoreApplication>
#include <QTextEdit>



class interface : public QWidget {
    Q_OBJECT

public:
    interface(QWidget *parent = nullptr);

private:
    QFrame *imageFrame;
    QLabel *imageLabel;
    QComboBox *parameterComboBox1;
    QComboBox *parameterComboBox2;
    QPushButton *runButton;
    QProcess *process;
    QTextEdit *outputConsole;

    void callPythonFunction(const QString &functionName, const QVariantList &args);
    void setupUI();
private slots:
    void runPythonScript();
    void displayPlot();
    void handleProcessOutput();
    void handleProcessError();
};

#endif // INTERFACE_H
