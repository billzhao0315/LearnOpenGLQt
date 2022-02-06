#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
uniform float offsetpos;
out vec3 outcolor;
out vec3 outpos;
void main()
{
gl_Position=vec4(aPos.x+offsetpos, -aPos.y,aPos.z, 1.0);
outcolor = aColor;
outpos=aPos;
};
