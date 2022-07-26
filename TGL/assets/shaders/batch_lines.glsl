#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProjection;

layout (location = 0) out vec4 v_Color;
layout (location = 1) out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int EntityID;

layout (location = 0) in vec4 v_Color;
layout (location = 1) in flat int v_EntityID;

void main()
{
	color = v_Color;
	EntityID = v_EntityID;
}
