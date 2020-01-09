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
	//smooth shading
	//v_normal = mat3(transpose(inverse(u_model))) * vec3(normalize(position));
	//flat shading
	v_normal = mat3(transpose(inverse(u_model))) * normalize(normal);
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};
//uniform Material material;

//uniform sampler2D u_texture0;
uniform sampler2D u_texture_diffuse_0;
uniform sampler2D u_texture_diffuse_1;


uniform vec3 u_colorBase;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_viewPosition;


in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;
void main()
{
	vec3 norm = normalize(v_normal);
	vec4 texColor = texture(u_texture_diffuse_0, v_textureCoord);
	//vec4 texColor1 = texture(u_texture_diffuse_1, v_textureCoord);
	
	vec3 userColor = vec3(texColor);

	vec3 lightPos = u_lightPosition;
	vec3 lightDir = normalize(lightPos - vec3(v_position));
		
	float ambientIntensity = 0.90f;
	vec3 ambient = u_lightColor * ambientIntensity;
	
	float diffuseInt = 0.40f;
	float diffuseIntensity = max(dot(norm,lightDir), 0.0f);
	vec3 diffuse = diffuseInt * u_lightColor * diffuseIntensity;
	
	float specularIntensity = 0.55f;
	vec3 camera = u_viewPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));
	//vec3 cameraDir = normalize(camera + norm);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));

	float spec = pow(max(dot(cameraDir, reflectDir), 0.0f), 128);
	//float spec = sin(length(distance(vec3(v_position), lightPos)));
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = specularIntensity * spec * u_lightColor * facing;

	vec3 output = userColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}



