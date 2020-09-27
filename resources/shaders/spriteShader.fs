#version 330 core
out vec4 FragColor;


//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, texCoord);
    //FragColor = vec4(1, 0, 0, 1);
    //color = vec4(1, 0, 0, 1);
}
