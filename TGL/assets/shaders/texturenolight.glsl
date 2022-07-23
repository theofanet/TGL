#shader vertex
#version 330 core
		
layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;
layout(location=2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform vec3 u_ViewPosition;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPosition;
out vec3 ViewPosition;

void main(){
	v_TexCoord = a_TexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * a_Normal;  
    FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
    ViewPosition = u_ViewPosition;
	gl_Position = u_ViewProjection * vec4(FragPosition, 1.0);
}

#shader fragment
#version 330 core

struct Material {
    vec4 color;
    vec3 specular;
    vec3 ambient;
    vec3 diffuse;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
};
		
layout(location=0) out vec4 color;

uniform Material u_Material;

in vec2 v_TexCoord;
in vec3 Normal;  
in vec3 FragPosition;  
in vec3 ViewPosition;


void main(){
    vec3 textColor = texture(u_Material.diffuseMap, v_TexCoord).rgb * u_Material.color.rgb * u_Material.ambient.rgb;
	color = vec4(textColor, 1.0);
}