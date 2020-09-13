#shader vertex
#version 330 core

layout(location=0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main(){
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

struct Material {
    vec4 color;
    vec3 specular;
    vec3 ambient;
    vec3 diffuse;
    float shininess;
};

layout(location=0) out vec4 color;
uniform Material u_Material;

void main(){
	color = u_Material.color;
}