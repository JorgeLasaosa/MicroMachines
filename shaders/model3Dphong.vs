#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
  
out vec3 ourColor; // Output a color to the fragment shader
out vec3 FragPos;  
out vec3 Normal;
  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	ourColor = color;
	gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(vec4(position, 1.0f));
    Normal =  mat3(transpose(inverse(model))) * normal;
}