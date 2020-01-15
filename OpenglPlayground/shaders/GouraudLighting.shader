#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

struct Material
{
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float sh;
};

uniform Material u_material;

uniform sampler2D u_texture_diffuse_1;
uniform sampler2D u_texture_specular_1;

uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_colorBase;
uniform vec3 u_cameraPosition;

uniform mat4 u_model;
uniform mat4 u_mvp;
uniform mat4 u_view;

out vec3 v_outputColor;

void main()
{
	gl_Position = u_mvp * position;
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, textureCoord);


	vec4 v_position = u_model * position;
	vec3 baseColor = u_colorBase * vec3(texDiffuseColor);


	//smooth shading
	vec3 v_normal = mat3(transpose(inverse(u_model))) * vec3(normalize(position));
	//flat shading
	//vec3 v_normal = mat3(transpose(inverse(u_model))) * normal;
	v_normal = normalize(v_normal);
	
	//calculate camera
	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));

	//calculate lighting
	vec3 lightDir = normalize(u_lightPosition - vec3(v_position));
	//vec3 reflectDir = normalize(reflect(-lightDir, v_normal));
	vec3 halfwayVec = normalize(lightDir + cameraDir);

	vec3 ambientIntensity = u_material.ka;
	vec3 ambient = ambientIntensity * u_lightColor;
	
	vec3 diffuseInt = u_material.kd;
	float diffuseIntensity = max(dot(v_normal, lightDir), 0.0f);
	vec3 diffuse = diffuseInt * diffuseIntensity * u_lightColor;

	vec3 specularIntensity = u_material.ks * vec3(texSpecularColor);
	float spec = pow(max(dot(cameraDir, halfwayVec), 0.0f), 128.0f * u_material.sh);
	//float spec = sin(length(distance(vec3(v_position), u_lightPosition)));
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = specularIntensity * spec * u_lightColor * facing;

	v_outputColor = baseColor * (ambient + diffuse) + specular;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_outputColor;

void main(void)
{
	color = vec4(v_outputColor, 1.0f);
}
