#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform float u_TexScale;
uniform vec4 u_Color;

void main()
{
	color = texture(u_Texture, v_texCoord * u_TexScale) * u_Color;
}

