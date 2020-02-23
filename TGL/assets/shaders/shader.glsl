#shader vertex
#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec3 a_Color;

out vec3 col;

void main(){
	gl_Position = vec4(a_Position, 1.0);
	col = a_Color;
}

#shader fragment
#version 330 core

layout(location=0) out vec4 color;
in vec3 col;

void main(){
	color = vec4(col, 1.0);
}