#version 330 core
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in float texID;

// uniform mat4 Model;
// uniform mat4 View;
// uniform mat4 Projection;
uniform mat4 MVP;

out vec4 aColour; // specify a color output to the fragment shader
out vec2 aTexCoord;
out float aTexID;

void main()
{
    gl_Position = MVP * pos;
    aColour = colour;
    aTexCoord = texCoord;
    aTexID = texID;
}
