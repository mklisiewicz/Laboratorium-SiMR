#include "interface.h"

interface::interface(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void interface::setupUI() {
    imageFrame = new QFrame(this);
    imageFrame->setFrameShape(QFrame::StyledPanel);
    imageFrame->setMinimumSize(1000, 500);

    parameterSliderA = new QSlider(Qt::Horizontal);
    parameterSliderA->setRange(-10, 10);  // Set the range of the slider as needed
    QTimer *debounceTimerA = new QTimer(this);
    debounceTimerA->setSingleShot(true);
    connect(debounceTimerA, &QTimer::timeout, this, [this]() {
        updateScatterDataA(parameterSliderA->value());
    });
    connect(parameterSliderA, &QSlider::valueChanged, this, &interface::updateScatterDataA);
    
    parameterSliderB = new QSlider(Qt::Horizontal);
    parameterSliderB->setRange(-10, 10);  // Set the range of the slider as needed
    QTimer *debounceTimerB = new QTimer(this);
    debounceTimerB->setSingleShot(true);
    connect(debounceTimerB, &QTimer::timeout, this, [this]() {
        updateScatterDataB(parameterSliderB->value());
    });
    connect(parameterSliderB, &QSlider::valueChanged, this, &interface::updateScatterDataB);

    frameLayout = new QVBoxLayout(imageFrame);
    
    
    parameterLineEdit1 = new QLineEdit(this);
    parameterLineEdit2 = new QLineEdit(this);


    runButton = new QPushButton("Run", this);
    connect(runButton, &QPushButton::clicked, this, &interface::runPythonScript);

    imageFrame->setLayout(frameLayout);

    setUpScatter();

    QHBoxLayout *comboBoxLayout = new QHBoxLayout;
    comboBoxLayout->addWidget(parameterLineEdit1);
    comboBoxLayout->addWidget(parameterLineEdit2);
    comboBoxLayout->addWidget(parameterSliderA);
    comboBoxLayout->addWidget(parameterSliderB);
    comboBoxLayout->addWidget(runButton);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(imageFrame);
    mainLayout->addLayout(comboBoxLayout);

    setLayout(mainLayout);
}

double interface::energy(double x, double v, double b) {
    return (b * x * x + v * v) / 2.0;
}

std::vector<std::vector<double>> interface::plotPhaseTrajectory(double a, double b) {
    System system{a, b};
    std::vector<double> x(2);
    x[0] = 5.0;
    x[1] = 2.0;

    double E_0 = energy(x[0], x[1], b);

    std::vector<std::vector<double>> x_vec;
    std::vector<double> times;
    double t = 0.0;
    double dt = 0.01;
    double t_end = 10.0;

    boost::numeric::odeint::integrate_const(boost::numeric::odeint::runge_kutta4<std::vector<double>>(), system, x, t, t_end, dt,
        [&x_vec, &times](const std::vector<double> &x, double t) {
            x_vec.push_back(x);
            times.push_back(t);
        });

    return x_vec;
}

void interface::setUpScatter(){
    scatter = new Q3DScatter();
    scatter->setMinimumSize(QSize(1000, 1000));
    scatter->setAspectRatio(1.0);
    
    scatter->setOptimizationHints(QAbstract3DGraph::OptimizationStatic);
    Q3DTheme *theme = scatter->activeTheme();
    theme->setBackgroundEnabled(false);
    theme->setLabelBackgroundEnabled(false);
    scatter->setShadowQuality(QAbstract3DGraph::ShadowQualityNone); // Disable shadows
    
    scatter->axisX()->setAutoAdjustRange(true);
    scatter->axisY()->setAutoAdjustRange(true);
    scatter->axisZ()->setAutoAdjustRange(true);
    scatter->axisX()->setTitle("x");
    scatter->axisZ()->setTitle("v");
    scatter->axisY()->setTitle("E/m");
    scatter->axisX()->setTitleVisible(true);
    scatter->axisZ()->setTitleVisible(true);
    scatter->axisY()->setTitleVisible(true);

    QWidget *container = QWidget::createWindowContainer(scatter);

    frameLayout->addWidget(container);
}

void interface::create3DPlot(const std::vector<std::vector<double>>& x_vec, double b) {
    
    
    while (scatter->seriesList().count() > 0) {
        scatter->removeSeries(scatter->seriesList().first());
    }

    QScatter3DSeries *series = new QScatter3DSeries();
    scatter->addSeries(series);

    // Add data to the series
    QScatterDataArray *data = new QScatterDataArray();
    data->resize(x_vec.size());

    for (int i = 0; i < x_vec.size(); ++i) {
        const auto &xi = x_vec[i];
        if (xi.size() >= 2) {
            (*data)[i] = QVector3D(xi[0], energy(xi[0], xi[1], b), xi[1]);
        } else {
            qWarning() << "Invalid data point at index" << i << ":" << xi;
        }
    }

    series->dataProxy()->resetArray(data);

    
}

void interface::updateScatterDataA(int value) {
    // Convert the slider value to the parameter value as needed
    double a = static_cast<double>(value) / 10.0;
    double b = static_cast<double>(parameterSliderB->value());
    // Update the scatter data
    std::vector<std::vector<double>> x_vec = plotPhaseTrajectory(a, b);
    create3DPlot(x_vec, b);
}

void interface::updateScatterDataB(int value) {
    // Convert the slider value to the parameter value as needed
    double a = static_cast<double>(parameterSliderB->value());
    double b = static_cast<double>(value) / 10.0;
    // Update the scatter data
    std::vector<std::vector<double>> x_vec = plotPhaseTrajectory(a, b);
    create3DPlot(x_vec, b);
}

void interface::runPythonScript() {
    QString param1 = parameterLineEdit1->text();
    QString param2 = parameterLineEdit2->text();

    bool ok1, ok2;
    double a = param1.toDouble(&ok1);
    double b = param2.toDouble(&ok2);

    if (ok1 && ok2) {
        std::vector<std::vector<double>> x_vec = plotPhaseTrajectory(a, b);
        
        create3DPlot(x_vec, b);
    }
}
