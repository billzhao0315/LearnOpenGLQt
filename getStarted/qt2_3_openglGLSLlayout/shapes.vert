#version 330 core
layout(location=2) in vec3 aPos;
out vec4 vertexcolor;
void main()
{
gl_Position=vec4(aPos.x, aPos.y,aPos.z, 1.0);
vertexcolor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};