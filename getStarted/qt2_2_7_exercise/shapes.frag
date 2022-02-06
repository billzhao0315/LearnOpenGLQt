#version 330 core
in vec3 outcolor;
out vec4 FragColor;
in vec3 outpos;
void main()
{
FragColor=vec4(outpos,1.0f);
};
