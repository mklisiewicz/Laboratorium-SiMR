#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QObject>
#include <QWindow>
#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QCoreApplication>
#include <QLineEdit>
#include <QAbstract3DInputHandler>
#include <boost/numeric/odeint.hpp>
#include <Eigen/Dense>
#include <gnuplot-iostream.h>
#include <QtDataVisualization>
#include <QScatter3DSeries>
#include <QScatterDataProxy>
#include <Q3DScatter>
#include <QVector3D>
#include <QtGraphs>
#include <QtCharts>
#include <Q3DTheme>
#include <QSlider>


class interface : public QWidget {
    Q_OBJECT

public:
    interface(QWidget *parent = nullptr);

private:
    QFrame *imageFrame;
    QLabel *imageLabel;
    QLineEdit *parameterLineEdit2;
    QLineEdit *parameterLineEdit1;
    QPushButton *runButton;
    Q3DScatter *scatter;
    QVBoxLayout *frameLayout;
    QSlider *parameterSliderA;
    QSlider *parameterSliderB;
    
    void setupUI();
    void setUpScatter();
    double energy(double x, double v, double b);
    std::vector<std::vector<double>> plotPhaseTrajectory(double a, double b);
    void create3DPlot(const std::vector<std::vector<double>>& x_vec, double b);
    void updateScatterDataA(int value);
    void updateScatterDataB(int value);
    struct System {
    double a, b;

    void operator()(const std::vector<double> &x, std::vector<double> &dxdt, double /* t */) {
        dxdt[0] = x[1];
        dxdt[1] = -a * x[1] - b * x[0];
    }
    };
private slots:
    void runPythonScript();
};

#endif // INTERFACE_H
