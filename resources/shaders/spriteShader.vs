#version 330 core
in vec3 aPos;
in vec2 aTexCoord;

// uniform mat4 Model;
// uniform mat4 View;
// uniform mat4 Projection;
uniform mat4 MVP;
uniform uint frameIndex;
uniform float x_uv;
uniform float y_uv;

//out vec4 vertexColor; // specify a color output to the fragment shader
out vec2 texCoord;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    //vertexColor = vec4(1, 0, 0, 1);
    // 8525/31
    texCoord = vec2((frameIndex + aTexCoord.x)*(x_uv), (aTexCoord.y)+(y_uv));
}
