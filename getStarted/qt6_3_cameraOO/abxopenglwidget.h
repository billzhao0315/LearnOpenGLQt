#ifndef ABXOPENGLWIDGET_H
#define ABXOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include "camera.h"

class ABXOpenglWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{eNone,eRect,eCircle, eTriangle};
    explicit ABXOpenglWidget(QWidget *parent = nullptr);
    ~ABXOpenglWidget();
    void drawShape(Shape shape);
    void setWireMode(bool mode);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
public slots:
    void on_timeout();
private:
    Shape m_shape;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture* m_textureWall;
    QOpenGLTexture* m_textureSmile;
    QTimer m_timer;
    QElapsedTimer m_time;
//    QVector3D m_cameraPosition;
//    QVector3D m_cameraFront;
//    QVector3D m_cameraRight;
//    QVector3D m_cameraTarget;
//    QVector3D m_cameraDirection;
    bool m_pressed;
    QPoint m_lastPos;
    Camera m_camera;
signals:

};

#endif // ABXOPENGLWIDGET_H
