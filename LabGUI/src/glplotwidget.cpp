#include "glplotwidget.h"
#include <Windows.h>
#include <QtMath>
#include <QPainter>
#include <QFont>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QDebug>
#include <iostream>

GLPlotWidget::GLPlotWidget(QWidget *parent)
    : QOpenGLWidget(parent), minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}


void GLPlotWidget::setTrajectoryData(const std::vector<std::vector<double>>& trajectory) {
    this->trajectoryData = trajectory;
    updateBoundingBox();
    updateProjection();
    update(); // Request redraw
}

void GLPlotWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void GLPlotWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLPlotWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    updateBoundingBox();

    glTranslatef(translateX, translateY, zoomLevel);

    // Apply rotations
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    // Draw the axes
    drawAxes();
    
    glBegin(GL_LINE_STRIP);
    double color_step = 1.0/maxZ;
    double color = color_step;
    for (const auto& point : trajectoryData) {
        color=point[2]*color_step;
        glColor3f(color, 1-color, 0.0f);
        if (point.size() >= 3) {
            glVertex3f(point[0], point[1], point[2]);
        }
    }
    glEnd();

    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12));
    painter.setRenderHint(QPainter::TextAntialiasing);

    // Example of drawing text at specific 2D positions
    painter.drawText(10, 20, "X Axis");
    painter.drawText(10, 40, "Y Axis");
    painter.drawText(10, 60, "Z Axis");
}

bool GLPlotWidget::project(float objX, float objY, float objZ, float& winX, float& winY) {
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    GLdouble winX_d, winY_d, winZ_d;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (gluProject(static_cast<GLdouble>(objX), static_cast<GLdouble>(objY), static_cast<GLdouble>(objZ),
                   modelview, projection, viewport, &winX_d, &winY_d, &winZ_d)) {
        // Invert Y coordinate because the OpenGL Y coordinate is from bottom to top
        winX = static_cast<float>(winX_d);
        winY = static_cast<float>(viewport[3] - winY_d);
        return true;
    }
    return false;
}


void GLPlotWidget::drawText(const QString& text, float x, float y, float z) {
    float winX, winY;
    if (project(x, y, z, winX, winY)) {
        QPainter painter(this);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 8));
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.drawText(static_cast<int>(winX), static_cast<int>(winY), text);
    }
}

void GLPlotWidget::drawAxes() {
    // Set line width for axes
    glLineWidth(2.0);

    glBegin(GL_LINES);

    // X axis in white
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-100, 0.0f, 0.0f);
    glVertex3f(100, 0.0f, 0.0f);

    // Y axis in white
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -100, 0.0f);
    glVertex3f(0.0f, 100, 0.0f);

    // Z axis in white
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100);
    glVertex3f(0.0f, 0.0f, 100);

    glEnd();

    // Reset line width
    glLineWidth(1.0);

    // Draw ticks and labels
    // You can customize this part further based on your requirements
    const float tickLength = 0.5f;
    const int numTicks = 50;
    const float tickSpacingX = 2*xRange / numTicks;
    const float tickSpacingY = 2*yRange / numTicks;
    const float tickSpacingZ = 2*zRange / numTicks;

    // Draw ticks along the X axis
    glBegin(GL_LINES);
    for (int i = 0; i <= numTicks; ++i) {
        float x = -xRange + i * tickSpacingX;
        glVertex3f(x, -tickLength, 0.0f);
        glVertex3f(x, tickLength, 0.0f);
    }
    glEnd();

    for (int i = 0; i <= numTicks; ++i) {
        float x = -xRange + i * tickSpacingX;
        (QString::number(x), x, -0.5 * tickLength, 0.0);
    }

    // Draw ticks along the Y axis
    glBegin(GL_LINES);
    for (int i = 0; i <= numTicks; ++i) {
        float y = -yRange + i * tickSpacingY;
        glVertex3f(-tickLength, y, 0.0f);
        glVertex3f(tickLength, y, 0.0f);
    }
    glEnd();

    // Draw labels for Y axis

    for (int i = 0; i <= numTicks; ++i) {
        float y = -yRange + i * tickSpacingY;
        drawText(QString::number(y), -0.5 * tickLength, y, 0.0);
    }

    // Draw ticks along the Z axis
    glBegin(GL_LINES);
    for (int i = 0; i <= numTicks; ++i) {
        float z = -zRange + i * tickSpacingZ;
        glVertex3f(0.0f, -tickLength, z);
        glVertex3f(0.0f, tickLength, z);
    }
    glEnd();

    // Draw labels for Z axis

    for (int i = 0; i <= numTicks; ++i) {
        float z = -zRange + i * tickSpacingZ;
        drawText(QString::number(z), 0.0, -0.5 * tickLength, z);
    }
}

void GLPlotWidget::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        zoomLevel += 1.0f; // Zoom in
    } else {
        zoomLevel -= 1.0f; // Zoom out
    }
    update(); // Request redraw
}

void GLPlotWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos(); // Save current mouse position

    // Check if right mouse button is pressed
    if (event->button() == Qt::RightButton) {
        // Set flag to indicate rotation around Z-axis
        isDragging = true;
    }
}

void GLPlotWidget::mouseMoveEvent(QMouseEvent *event) {
    // Calculate change in mouse position
    QPoint diff = event->pos() - lastMousePos;

    // Check if right mouse button is pressed and rotation around Z-axis is enabled
    if (event->buttons() & Qt::RightButton && isDragging) {
        // Update rotation angle around Z-axis based on horizontal mouse movement
        translateX += diff.x() * translationSpeed;
        translateY += diff.y() * translationSpeed;
    } else {
        // Update rotation angles based on mouse movement for other cases
        rotationX += diff.y();
        rotationZ += diff.x();
    }

    // Request a redraw
    update();

    // Update last mouse position
    lastMousePos = event->pos();
}

double GLPlotWidget::energy(double x, double v, double b) {
    return (b * x * x + v * v) / 2.0;
}

void GLPlotWidget::updateBoundingBox() {
    if (trajectoryData.empty()) return;

    minX = maxX = trajectoryData[0][0];
    minY = maxY = trajectoryData[0][1];
    minZ = maxZ = trajectoryData[0][2];

    for (const auto& point : trajectoryData) {
        if (point[0] < minX) minX = point[0];
        if (point[0] > maxX) maxX = point[0];
        if (point[1] < minY) minY = point[1];
        if (point[1] > maxY) maxY = point[1];
        if (point[2] < minZ) minZ = point[2];
        if (point[2] > maxZ) maxZ = point[2];
    }

    centerX = (minX + maxX) / 2.0;
    centerY = (minY + maxY) / 2.0;
    centerZ = (minZ + maxZ) / 2.0;
}

void GLPlotWidget::updateProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate bounding box dimensions
    double rangeX = maxX - minX;
    double rangeY = maxY - minY;
    double rangeZ = maxZ - minZ;

    // Calculate maximum range
    double maxRange = qMax(qMax(rangeX, rangeY), rangeZ);

    // Calculate aspect ratio
    double aspect = (double)width() / (double)height();

    // Calculate the aspect-scaled bounding box
    double scaledRangeX = maxRange * aspect;
    double scaledRangeY = maxRange;
    double scaledRangeZ = maxRange;

    // Set up orthographic projection based on the scaled bounding box
    glOrtho(-scaledRangeX / 2.0, scaledRangeX / 2.0,
            -scaledRangeY / 2.0, scaledRangeY / 2.0,
            -scaledRangeZ / 2.0, scaledRangeZ / 2.0);

    glMatrixMode(GL_MODELVIEW);
}
