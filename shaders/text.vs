#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 texCoordsOut;

uniform mat4 projection;

void main()
{
    texCoordsOut = texCoords;
    gl_Position = projection * vec4(position, 1.0);
}  
