#include "abxopenglwidget.h"
#include <QDebug>

unsigned int VBOs[2], VAOs[2];
float firstTriangle[]={
    -0.9f, -0.5f, 0.0f,
    0.0f, -0.5f, 0.0f,
    -0.45f, 0.5f, 0.0f,
};

float secondTriangle[]={
    0.0f, -0.5f, 0.0f,
    0.9f, -0.5f, 0.0f,
    0.45f, 0.5f, 0.0f
};

const char* vertextShaderSource1="#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(aPos.x, aPos.y,aPos.z, 1.0);\n"
"}\0;";
const char* fragmentShaderSource1="#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor=vec4(1.0f, 0.5f,0.2f,1.0f);\n"
"}\n\0";

unsigned int shaderProgram1;

const char* vertextShaderSource2="#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(aPos.x, aPos.y,aPos.z, 1.0);\n"
"}\0;";
const char* fragmentShaderSource2="#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor=vec4(1.0f, 1.0f,0.0f,1.0f);\n"
"}\n\0";

unsigned int shaderProgram2;
ABXOpenglWidget::ABXOpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void ABXOpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //create VBO and VAO

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //bind
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);


    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof (float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);


    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof (float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    shaderProgram1 = generateShaderProgram(vertextShaderSource1, fragmentShaderSource1);
    shaderProgram2 = generateShaderProgram(vertextShaderSource2, fragmentShaderSource2);
}

void ABXOpenglWidget::resizeGL(int w, int h)
{

}

void ABXOpenglWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,0.1f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES,0,3);
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES,0,3);
}

unsigned int ABXOpenglWidget::generateShaderProgram(const char *vertexShaderSource, const char *fragShaderSource)
{
    unsigned int vertexShader;
    unsigned int fragShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char logInfo[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, logInfo);
        qDebug()<<"ERROR:"<< logInfo;
    }

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragShader, 512, NULL, logInfo);
        qDebug()<<"ERROR:"<< logInfo;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, logInfo);
        qDebug()<<"ERROR:"<<logInfo;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}
