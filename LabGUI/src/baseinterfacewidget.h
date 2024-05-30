#ifndef BASEINTERFACEWIDGET_H
#define BASEINTERFACEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QTabWidget>

class BaseInterfaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseInterfaceWidget(QWidget *parent = nullptr);

protected:
    QHBoxLayout *mainLayout; // Common layout for the widget
    QTabWidget *tabWidget; // Tab widget for simulation and measurement parts
    QWidget *simulationWidget; // Widget for simulation part
    QWidget *measurementWidget; // Widget for measurement part
};

#endif // BASEINTERFACEWIDGET_H
