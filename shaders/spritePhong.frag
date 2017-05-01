#version 330 core
out vec4 color;

in vec2 texCoordsOut;
in vec3 FragPos;
in vec3 Normal;  
  
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos; 

uniform sampler2D image;

void main() {
    // Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    color = vec4(ambient + diffuse + specular, 1.0f) * texture(image, texCoordsOut);
    //color = vec4(specular, 1.0f) * texture(image, texCoordsOut);
}
