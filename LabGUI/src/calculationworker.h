#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <vector>
#include <boost/numeric/odeint.hpp>

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(QObject *parent = nullptr) : QObject(parent) {}

    void setParameters(double a, double b) {
        m_a = a;
        m_b = b;
    }

public slots:
    void process() {
        std::vector<std::vector<double>> result = plotPhaseTrajectory(m_a, m_b);
        emit resultReady(result);
    }

signals:
    void resultReady(const std::vector<std::vector<double>> &result);

private:
    double m_a;
    double m_b;

    struct System {
    double a, b;


    void operator()(const std::vector<double> &x, std::vector<double> &dxdt, double /* t */) {
        dxdt[0] = x[1];
        dxdt[1] = -a * x[1] - b * x[0];
    }
    };

    double energy(double x, double v, double b) {
        return (b * x * x + v * v) / 2.0;
    }

    std::vector<std::vector<double>> plotPhaseTrajectory(double a, double b) {
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
};
#endif