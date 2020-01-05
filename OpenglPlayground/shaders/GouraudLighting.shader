#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_mvp;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_colorBase;
uniform vec3 u_viewPosition;

out vec3 v_outputColor;

void main()
{
	gl_Position = u_mvp * position;
	
	vec4 v_position = u_model * position;
	vec3 v_normal = mat3(transpose(inverse(u_model))) * vec3(normalize(position));
	//vec3 v_normal = mat3(transpose(inverse(u_model))) * normal;
	v_normal = normalize(v_normal);
	
	//calculate camera
	vec3 camera = u_viewPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));
	//vec3 cameraDir = normalize(camera + v_normal);

	//calculate lighting
	vec3 lightDir = normalize(u_lightPosition - vec3(v_position));
	vec3 reflectDir = normalize(reflect(-lightDir, v_normal));

	float ambientIntensity = 0.20f;
	vec3 ambient = ambientIntensity * u_lightColor;
	
	float diffuseInt = 0.40f;
	float diffuseIntensity = max(dot(v_normal, lightDir), 0.0f);
	vec3 diffuse = diffuseInt * diffuseIntensity * u_lightColor;

	float specularIntensity = 0.55f;
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0f), 128);
	//float spec = sin(length(distance(vec3(v_position), u_lightPosition)));
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = specularIntensity * spec * u_lightColor * facing;

	v_outputColor = u_colorBase * (ambient + diffuse) + specular;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_outputColor;

void main(void)
{
	color = vec4(v_outputColor, 1.0f);
}
