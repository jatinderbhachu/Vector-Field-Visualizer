#version 330 core
#define MAX_TEXTURE_UNITS 32
layout (location = 0) out vec4 color;

in vec4 aColour; // specify a color output to the fragment shader
in vec2 aTexCoord;
in float aTexID;

uniform sampler2D textures[MAX_TEXTURE_UNITS];

void main()
{
    int index = int(aTexID);
    switch(index){
        case 0: color = texture(textures[0], aTexCoord) * aColour; break;
        case 1: color = texture(textures[1], aTexCoord) * aColour; break;
        case 2: color = texture(textures[2], aTexCoord) * aColour; break;
        case 3: color = texture(textures[3], aTexCoord) * aColour; break;
        case 4: color = texture(textures[4], aTexCoord) * aColour; break;
        case 5: color = texture(textures[5], aTexCoord) * aColour; break;
        case 6: color = texture(textures[6], aTexCoord) * aColour; break;
        case 7: color = texture(textures[7], aTexCoord) * aColour; break;
        case 8: color = texture(textures[8], aTexCoord) * aColour; break;
        case 9: color = texture(textures[9], aTexCoord) * aColour; break;
    }

    //color = aColour;
}
