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
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec4 color;
    vec3 specular;
    vec3 ambient;
    vec3 diffuse;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

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

uniform Light[10] u_Light;
uniform int u_LightsCount;
uniform Material u_Material;

in vec2 v_TexCoord;
in vec3 Normal;  
in vec3 FragPosition;  
in vec3 ViewPosition;


void main(){
    vec3 textColor = texture(u_Material.diffuseMap, v_TexCoord).rgb;

    vec3 materialSpecular = texture(u_Material.specularMap, v_TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(ViewPosition - FragPosition);

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 lightDir = vec3(0.0);
    float attenuation = 1.0f;

    for(int i=0;i<u_LightsCount;i++){
        if(u_Light[i].type == 1){
            lightDir = normalize(-u_Light[i].direction);
            attenuation = 1.0f;
        }
        else{
            lightDir = normalize(u_Light[i].position - FragPosition);
            float distance = length(u_Light[i].position - FragPosition);
            attenuation = 1.0 / (u_Light[i].constant + u_Light[i].linear * distance + u_Light[i].quadratic * (distance * distance));    
        }
                
        float diff = max(dot(norm, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess * 128.0);

        vec3 lAmbient = u_Light[i].ambient * textColor;
        vec3 lDiffuse = diff * textColor * u_Light[i].diffuse;
        vec3 lSpecular = spec * materialSpecular * u_Light[i].specular;

        if(u_Light[i].type == 3){
            float theta = dot(lightDir, normalize(-u_Light[i].direction));
            float epsilon = u_Light[i].cutOff - u_Light[i].outerCutOff;
            float intensity = clamp((theta - u_Light[i].outerCutOff) / epsilon, 0.0, 1.0); 

            lDiffuse *= intensity;
            lSpecular *= intensity;
        }

        lAmbient *= attenuation;
        lDiffuse *= attenuation;
        lSpecular *= attenuation;
        
        ambient += lAmbient;
        diffuse += lDiffuse;
        specular += lSpecular;   
    }

    vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
}