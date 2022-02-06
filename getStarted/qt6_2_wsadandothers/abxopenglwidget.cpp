#include "abxopenglwidget.h"
#include <QDebug>

unsigned int VBO, VAO;
float vertices[]={
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[]={
    0,1,3,
    1,2,3
};

QVector<QVector3D> cubePositions = {
  QVector3D( 0.0f,  0.0f,  0.0f),
  QVector3D( 2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D( 1.3f, -2.0f, -2.5f),
  QVector3D( 1.5f,  2.0f, -2.5f),
  QVector3D( 1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};

//const char* vertextShaderSource="#version 330 core\n"
//"layout(location=0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"gl_Position=vec4(aPos.x, aPos.y,aPos.z, 1.0);\n"
//"}\0;";
//const char* fragmentShaderSource="#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"FragColor=vec4(1.0f, 0.5f,0.2f,1.0f);\n"
//"}\n\0";

//unsigned int shaderProgram;
unsigned int EBO;
float ratio = 0.5;
QPoint deltaPos;
float fov = 45.0;
#define PI 3.1415926
#define TIMEOUTSECONDS 100
ABXOpenglWidget::ABXOpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_pressed = false;
    m_lastPos = QPoint(width()/2, height()/2);
    m_cameraFront = QVector3D(0.0,0.0,-1.0);
    m_cameraPosition = QVector3D(0.0,0.0,5.0);
    m_cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
    m_cameraDirection = QVector3D(m_cameraPosition - m_cameraTarget);
    m_cameraDirection.normalize();
    QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);

    m_cameraRight =QVector3D::crossProduct(up, m_cameraDirection);
    m_cameraRight.normalize();

    m_shape = Shape::eNone;
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    m_timer.start(TIMEOUTSECONDS);
    m_time.start();
}

ABXOpenglWidget::~ABXOpenglWidget()
{
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    //glDeleteProgram(shaderProgram);
    doneCurrent();
}

void ABXOpenglWidget::drawShape(Shape shape)
{
    m_shape = shape;
    update();
}

void ABXOpenglWidget::setWireMode(bool mode)
{
    makeCurrent();
    if(mode){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}

void ABXOpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //create VBO and VAO

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 5*sizeof (float),(void*)0);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 5*sizeof (float),(void*)(3*sizeof (float)));

//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    m_textureWall = new QOpenGLTexture(QImage(":/images/image/wall.jpg").mirrored());
    m_textureSmile = new QOpenGLTexture(QImage(":/images/image/awesomeface.png").mirrored());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); //VAO will record glBindBuffer of GL_ELEMENT_ARRAY_BUFFER, not for GL_ARRAY_BUFFER
    glBindVertexArray(0);

    bool success;
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shapes.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shapes.frag");
    success = m_shaderProgram.link();
    if(!success){
        qDebug()<<"ERROR:"<<m_shaderProgram.log();
    }
    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("textureWall",0);
    m_shaderProgram.setUniformValue("textureSmile",1);




//    unsigned int vertexShader;
//    unsigned int fragShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertextShaderSource, NULL);
//    glCompileShader(vertexShader);
//    int success;
//    char logInfo[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if(!success){
//        glGetShaderInfoLog(vertexShader, 512, NULL, logInfo);
//        qDebug()<<"ERROR:"<< logInfo;
//    }

//    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragShader);
//    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
//    if(!success){
//        glGetShaderInfoLog(fragShader, 512, NULL, logInfo);
//        qDebug()<<"ERROR:"<< logInfo;
//    }

//    //unsigned int shaderProgram;
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragShader);
//    glLinkProgram(shaderProgram);
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if(!success){
//        glGetProgramInfoLog(shaderProgram, 512, NULL, logInfo);
//        qDebug()<<"ERROR:"<<logInfo;
//    }

//    glDeleteShader(vertexShader);
//    glDeleteShader(fragShader);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void ABXOpenglWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}


void ABXOpenglWidget::paintGL()
{
    //unsigned int time = QTime::currentTime().second();
    glClearColor(0.2f,0.3f,0.3f,0.1f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //glUseProgram(shaderProgram);
    m_shaderProgram.bind();
   float time1 =  m_time.elapsed()/10.0;//use 1000.0 instead of 1000
//   float radius = 10.0f;
//   float camX = sin(time1) * radius;
//   float camZ = cos(time1) * radius;

    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES,0,6);
    m_shaderProgram.setUniformValue("ratio", ratio);
    switch (m_shape) {
    case Shape::eRect:
    {
        //unsigned int wallTextureId =  m_shaderProgram.uniformLocation("textureWall");
        //qDebug()<<"texture id:"<< wallTextureId;
        m_textureWall->bind(0);
        //unsigned int smileTextureId =  m_shaderProgram.uniformLocation("textureSmile");
        //qDebug()<<"texture id:"<< smileTextureId;
        m_textureSmile->bind(1);
        //float bordercolor[]={1.0f, 1.0f,0.0f,1.0f};
        //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        QMatrix4x4 projection;
        projection.perspective(fov,(float)width()/height(), 0.1, 100);
        QMatrix4x4 view;
        view.lookAt(m_cameraPosition, m_cameraPosition+m_cameraFront, QVector3D(0.0,1.0,0.0));
        foreach(auto item, cubePositions){
            QMatrix4x4 model;//rotation firstly, then tranlate secondly, Note:the sequence


            QMatrix4x4 mat;
        //        mat.translate(0.5, 0.0,0.0);
        //        mat.rotate(-10, 0.0,0.0,1.0);
            //projection.ortho(-1.0, 1.0, -1.0, 1.0, 0.1, 4);

            //view.translate(item);

            model.translate(item);
            model.rotate(time1, 1.0,0.5,0.5);
            //model.rotate(-45,0.0, 1.0, 0.0);
            mat = projection*view*model;
            m_shaderProgram.setUniformValue("rotation", mat);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0/*&indices*/);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        break;
    }
    default:
        break;
    }


}
#include <QKeyEvent>
void ABXOpenglWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 2.5 * TIMEOUTSECONDS/1000.0;
    switch (event->key()) {
    case Qt::Key_Up:
    {
        ratio += 0.1;
        if(ratio > 1.0){
            ratio = 1;
        }
        update();
        break;
    }
    case Qt::Key_Down:
    {
        ratio -= 0.1;
        if(ratio < 0){
            ratio = 0;
        }
        update();
        break;
    }
    case Qt::Key_W:{
        m_cameraPosition += cameraSpeed * m_cameraFront;break;
        update();
    }
    case Qt::Key_S:{
        m_cameraPosition -= cameraSpeed * m_cameraFront;break;
        update();
    }
    case Qt::Key_D:{
        m_cameraPosition += cameraSpeed * m_cameraRight;break;
        update();
    }
    case Qt::Key_A:{
        m_cameraPosition -= cameraSpeed * m_cameraRight;break;
        update();
    }
    }
}

void ABXOpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_pressed){
        return;
    }

    static float yaw = -90;
    static float pitch=0;
    //static QPoint lastPos(width()/2, height()/2);
    auto currentPos = event->pos();
    deltaPos = currentPos - m_lastPos;
    m_lastPos = currentPos;
    float sensitivity = 0.1f;
    deltaPos *= sensitivity;
    yaw -= deltaPos.x();
    pitch += deltaPos.y();
    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    //qDebug()<<"pitch angle:"<< pitch;
    m_cameraFront.setX(cos(yaw*PI/180)*cos(pitch*PI/180));
    m_cameraFront.setY(sin(pitch*PI/180));
    m_cameraFront.setZ(sin(yaw*PI/180)*cos(pitch*PI/180));
    m_cameraFront.normalize();
    update();
}

void ABXOpenglWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"mouse press event:"<< event->type();
    if(event->type() == QEvent::Type::MouseButtonPress){
        qDebug()<<"mousePress";
        m_lastPos = event->pos();
        m_pressed = true;
    }

}

void ABXOpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::Type::MouseButtonRelease){
        qDebug()<<"mouse Release";
        m_pressed = false;
    }
}


void ABXOpenglWidget::wheelEvent(QWheelEvent *event)
{
    //qDebug()<<"wheelEevent:"<< event->angleDelta();
    if(fov >= 1.0f && fov <= 75.0f){
        fov -= event->angleDelta().y()/120;
    }
    if(fov <= 1.0f) fov = 1.0f;
    if(fov >= 75.0f) fov = 75.0f;
    update();
}

void ABXOpenglWidget::on_timeout()
{
    update();
}
