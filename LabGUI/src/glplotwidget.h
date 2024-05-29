#ifndef GLPLOTWIDGET_H
#define GLPLOTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QtMath>
#include <QMouseEvent>

class GLPlotWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    GLPlotWidget(QWidget *parent = nullptr);

    void setTrajectoryData(const std::vector<std::vector<double>>& trajectory);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    double a;
    double b;
    std::vector<std::vector<double>> trajectoryData;
    QPoint lastMousePos;
    bool isDragging = false;
    
    int rotationX = -45;
    int rotationY = 0;
    int rotationZ = 45;

    float translateX = 0.0f;
    float translateY = 0.0f;
    float zoomLevel = -50.0f;
    float translationSpeed = -0.005f;

    int xRange = 50;
    int yRange = 50;
    int zRange = 50;

    double minX, maxX, minY, maxY, minZ, maxZ;
    double centerX, centerY, centerZ;

    double energy(double x, double v, double b);
    void drawAxes();
    void drawText(const QString& text, float x, float y, float z);
    bool project(float objX, float objY, float objZ, float& winX, float& winY);
    void updateBoundingBox();
    void updateProjection();
};


#endif // GLPLOTWIDGET_H
