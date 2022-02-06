#version 330 core
in vec3 outcolor;
out vec4 FragColor;
in vec2 outTextCoord;
uniform sampler2D textureWall;
uniform sampler2D textureSmile;
void main()
{
//FragColor=vec4(outcolor,1.0f);
    //FragColor=texture2D(textureWall, outTextCoord);
    FragColor= mix(texture(textureWall, outTextCoord), texture(textureSmile, outTextCoord), 0.5);
    //FragColor=texture(textureSmile, outTextCoord);
};
