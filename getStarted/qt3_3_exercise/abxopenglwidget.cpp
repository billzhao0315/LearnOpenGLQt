#include "abxopenglwidget.h"
#include <QDebug>

unsigned int VBO, VAO;
float vertices[]={
    //posiiton       //colors       //texture coords
    0.9f, 0.9f, 0.0f,1.0f,0.0f,0.0f,1.0f,1.0f, //top right
    0.9f, -0.9f, 0.0f,0.0f,1.0f,0.0f,1.0f,0.0f, //bottom right
    -0.9f, -0.9f, 0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,//bottom left
    -0.9f, 0.9f, 0.0f,0.5f,0.5f,0.5f,0.0f,1.0f  //top left
};

unsigned int indices[]={
    0,1,3,
    1,2,3
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
ABXOpenglWidget::ABXOpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_shape = Shape::eNone;
    setFocusPolicy(Qt::StrongFocus);
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
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8*sizeof (float),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof (float),(void*)(3*sizeof (float)));
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof (float),(void*)(6*sizeof (float)));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_textureWall = new QOpenGLTexture(QImage(":/images/image/wall.jpg").mirrored());
    m_textureSmile = new QOpenGLTexture(QImage(":/images/image/awesomeface.png").mirrored());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
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
    glClearColor(0.2f,0.3f,0.3f,0.1f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(shaderProgram);
    m_shaderProgram.bind();
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES,0,6);
    m_shaderProgram.setUniformValue("ratio", ratio);
    switch (m_shape) {
    case Shape::eRect:
    {
        unsigned int wallTextureId =  m_shaderProgram.uniformLocation("textureWall");
        qDebug()<<"texture id:"<< wallTextureId;
        m_textureWall->bind(wallTextureId);
        unsigned int smileTextureId =  m_shaderProgram.uniformLocation("textureSmile");
        qDebug()<<"texture id:"<< smileTextureId;
        m_textureSmile->bind(1);
        //float bordercolor[]={1.0f, 1.0f,0.0f,1.0f};
        //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0/*&indices*/);
        break;
    }
    default:
        break;
    }


}
#include <QKeyEvent>
void ABXOpenglWidget::keyPressEvent(QKeyEvent *event)
{
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

    }
}
