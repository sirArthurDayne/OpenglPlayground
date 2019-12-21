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

uniform float u_time;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform vec3 u_colorBase;

in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;
void main()
{
	vec4 texColor = texture(u_texture0, v_textureCoord);
	vec4 texColor1 = texture(u_texture1, v_textureCoord);
	vec3 UserColor = u_colorBase;
	vec3 LightColor = vec3(1.f);
	color = vec4(u_colorBase * LightColor, 1.0f);
}



