#include "baseinterfacewidget.h"

BaseInterfaceWidget::BaseInterfaceWidget(QWidget *parent) : QWidget(parent)
{
    // Create main layout
    mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Create tabs for simulation and measurement parts
    simulationWidget = new interface(this);
    measurementWidget = new QWidget(this);
    tabWidget->addTab(simulationWidget, "Simulation");
    tabWidget->addTab(measurementWidget, "Measurement");
}