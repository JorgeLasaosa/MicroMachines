#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 texCoordsOut;

uniform mat4 model;
uniform mat4 projection;

void main() {
	texCoordsOut = texCoords;
	gl_Position = projection * model * vec4(position, 1.0);
}
