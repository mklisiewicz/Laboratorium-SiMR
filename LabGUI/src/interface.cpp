#include "interface.h"

interface::interface(QWidget *parent) : QWidget(parent) {
    setupUI();

}

void interface::setupUI() {
    imageFrame = new QFrame(this);
    imageFrame->setFrameShape(QFrame::StyledPanel);
    imageFrame->setMinimumSize(1000, 500);

    dampingSlider = new QSlider(Qt::Horizontal);
    dampingSlider->setRange(-100, 100);  // Set the range of the slider as needed
    
    stiffnessSlider = new QSlider(Qt::Horizontal);
    stiffnessSlider->setRange(-1000, 1000);  // Set the range of the slider as needed

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(-2000, 2000);

    velocitySlider = new QSlider(Qt::Horizontal);
    velocitySlider->setRange(-2000, 2000);

    dampingSlider->setValue(5);
    stiffnessSlider->setValue(15);
    positionSlider->setValue(5);
    velocitySlider->setValue(15);

    connect(dampingSlider, &QSlider::valueChanged, this, &interface::updateParameters);
    connect(stiffnessSlider, &QSlider::valueChanged, this, &interface::updateParameters);
    connect(positionSlider, &QSlider::valueChanged, this, &interface::updateParameters);
    connect(velocitySlider, &QSlider::valueChanged, this, &interface::updateParameters);

    frameLayout = new QVBoxLayout(imageFrame);

    plotWidget = new GLPlotWidget();
    frameLayout->addWidget(plotWidget);
    updateParameters();

    imageFrame->setLayout(frameLayout);


    QVBoxLayout *slidersLayout = new QVBoxLayout;
    QHBoxLayout *parametersLayout = new QHBoxLayout;
    QHBoxLayout *initialConditionsLayout = new QHBoxLayout;

    parametersLayout->addWidget(dampingSlider);
    parametersLayout->addWidget(stiffnessSlider);
    initialConditionsLayout->addWidget(positionSlider);
    initialConditionsLayout->addWidget(velocitySlider);
    slidersLayout->addLayout(parametersLayout);
    slidersLayout->addLayout(initialConditionsLayout);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(imageFrame);
    mainLayout->addLayout(slidersLayout);

    setLayout(mainLayout);
}

double interface::energy(double x, double v, double b) {
    return (b * x * x + v * v) / 2.0;
}



std::vector<std::vector<double>> interface::plotPhaseTrajectory(double a, double b, double x_0, double v_0) {
    System system{a, b};
    std::vector<double> x(2);
    x[0] = x_0;
    x[1] = v_0;

    double E_0 = energy(x[0], x[1], b);

    std::vector<std::vector<double>> x_vec;
    std::vector<double> times;
    double t = 0.0;
    double dt = 0.1;
    double t_end = 100.0;

    boost::numeric::odeint::integrate_const(boost::numeric::odeint::runge_kutta4<std::vector<double>>(), system, x, t, t_end, dt,
        [&x_vec, &times, b](const std::vector<double> &x, double t) {
            // Store state variables at each time step
            x_vec.push_back(x);
            times.push_back(t);
            
            // Calculate and store the energy at each time step
            double energy_val = (b * x[0] * x[0] + x[1] * x[1]) / 2.0;
            x_vec.back().push_back(energy_val);
        });

    return x_vec;
}

void interface::updateParameters() {
    // Update parameter values
    a = static_cast<double>(dampingSlider->value()) / 1000.0;
    b = static_cast<double>(stiffnessSlider->value()) / 100.0;
    x_0 = static_cast<double>(positionSlider->value()) / 100.0;
    v_0 = static_cast<double>(velocitySlider->value()) / 100.0;
    
    // Compute trajectory
    std::vector<std::vector<double>> trajectory = plotPhaseTrajectory(a, b, x_0, v_0);

    // Update plot widget with new data
    plotWidget->setTrajectoryData(trajectory);
}
