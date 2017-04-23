#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;	// The color variable has attribute position 1
layout (location = 2) in vec2 texCoords;
  
out vec3 ourColor; // Output a color to the fragment shader

uniform vec3 color;
uniform mat4 model;
uniform mat4 projection;

void main() {
	ourColor = color;
	gl_Position = projection * model * vec4(position, 1.0);
}

