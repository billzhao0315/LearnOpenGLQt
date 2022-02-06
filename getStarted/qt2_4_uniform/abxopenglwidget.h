#ifndef ABXOPENGLWIDGET_H
#define ABXOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>

class ABXOpenglWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{eNone,eRect,eCircle, eTriangle};
    explicit ABXOpenglWidget(QWidget *parent = nullptr);
    ~ABXOpenglWidget();
    void drawShape(Shape shape);
    void setWireMode(bool mode);
public slots:
    void on_timeout();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    Shape m_shape;
    QOpenGLShaderProgram m_shaderProgram;
    QTimer m_timer;
signals:

};

#endif // ABXOPENGLWIDGET_H
