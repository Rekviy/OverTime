#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
	
uniform mat4 u_ProjView;
uniform mat4 u_Transform;			
out vec2 v_texCoord;

void main()
{
	v_texCoord = texCoord;
	gl_Position = u_ProjView * u_Transform * vec4(position, 1.0);
}