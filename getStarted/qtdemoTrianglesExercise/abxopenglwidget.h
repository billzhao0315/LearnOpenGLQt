#ifndef ABXOPENGLWIDGET_H
#define ABXOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class ABXOpenglWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit ABXOpenglWidget(QWidget *parent = nullptr);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    unsigned int generateShaderProgram(const char* vertexShaderSource, const char* fragShaderSource);
signals:

};

#endif // ABXOPENGLWIDGET_H
