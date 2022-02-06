#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTextCoord;
uniform mat4 rotation;
out vec2 outTextCoord;
void main()
{
gl_Position= rotation * vec4(aPos.x, aPos.y,aPos.z, 1.0);
outTextCoord = vec2(1.0 - aTextCoord.s, aTextCoord.t);
};
