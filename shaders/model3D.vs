#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec3 color;
  
out vec3 ourColor; // Output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	ourColor = color;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}