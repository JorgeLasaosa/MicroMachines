#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 texCoordsOut;
out vec3 FragPos;  
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objPos;

uniform vec4 frame;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	texCoordsOut = vec2(texCoords.x, 1.0f - texCoords.y)*frame.zw + frame.xy;
    FragPos = objPos + vec3(vec4(position, 1.0f));
    Normal = normal;
}
