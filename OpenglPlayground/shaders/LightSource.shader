#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_mvp;

out vec4 v_position;
out vec2 v_textureCoord;
out vec3 v_normal;

void main()
{
	gl_Position = u_mvp * position;
	v_position = position;
	v_textureCoord = textureCoord;
	v_normal = normal;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_lightColor;
in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;

void main()
{
	vec3 light = u_lightColor;
	color = vec4(light, 1.0f);
}