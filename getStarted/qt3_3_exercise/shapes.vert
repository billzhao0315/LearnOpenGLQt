#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in vec2 aTextCoord;
out vec3 outcolor;
out vec2 outTextCoord;
void main()
{
gl_Position=vec4(aPos.x, aPos.y,aPos.z, 1.0);
outcolor = aColor;
outTextCoord = vec2(1.0 - aTextCoord.s, aTextCoord.t);
};
