#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_mvp;

out vec4 v_position;
out vec2 v_textureCoord;
out vec3 v_normal;

void main()
{
	gl_Position = u_mvp * position;
	v_position = u_model * position;
	v_textureCoord = textureCoord;
	v_normal = mat3(transpose(inverse(u_model))) * normal;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_time;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform vec3 u_colorBase;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;

in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;
void main()
{
	//vec4 texColor = texture(u_texture0, v_textureCoord);
	//vec4 texColor1 = texture(u_texture1, v_textureCoord);
	
	vec3 UserColor = u_colorBase;
	vec3 norm = normalize(v_normal);

	vec3 lightPos = u_lightPosition;
	vec3 lightDir = normalize(lightPos - vec3(v_position));
	
	float ambientIntensity = 1.0f;
	vec3 ambient = u_lightColor * ambientIntensity;
	
	float diffuseIntensity = max(dot(norm,lightDir), 0.0f);
	vec3 diffuse = u_lightColor * diffuseIntensity;
	

	float specularIntensity = 0.25f;

	vec3 output = u_colorBase * (ambient + diffuse);

	color = vec4(output, 1.0f);
}



