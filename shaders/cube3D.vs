#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 texCoordsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 frame;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	//texCoordsOut = vec2(texCoords.s, 1.0f - texCoords.t);
	texCoordsOut = vec2(texCoords.x, 1.0f - texCoords.y)*frame.zw + frame.xy;
}
