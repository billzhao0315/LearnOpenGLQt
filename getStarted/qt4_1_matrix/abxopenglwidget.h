#ifndef ABXOPENGLWIDGET_H
#define ABXOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

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
private:
    Shape m_shape;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLTexture* m_textureWall;
    QOpenGLTexture* m_textureSmile;
signals:

};

#endif // ABXOPENGLWIDGET_H
