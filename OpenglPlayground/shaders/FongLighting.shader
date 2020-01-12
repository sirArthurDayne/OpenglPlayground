#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_mvp;

out vec4 v_position;
out vec2 v_textureCoord;
out vec3 v_normal;

void main()
{
	//WORLD SPACE CALCULATIONS
	v_position = u_model * position;
	v_textureCoord = textureCoord;
	
	//smooth shading
	//v_normal = mat3(transpose(inverse(u_model))) * vec3(normalize(position));
	//flat shading
	v_normal = mat3(transpose(inverse(u_model))) * normalize(normal);
	
	
	gl_Position = u_mvp * position;
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

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

uniform vec3 u_colorBase;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_cameraPosition;


in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;

void main()
{
	vec3 norm = normalize(v_normal);
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);

	vec3 baseColor = u_colorBase * vec3(texDiffuseColor);

	vec3 lightPos = u_lightPosition;
	vec3 lightDir = normalize(lightPos - vec3(v_position));
	
	vec3 ambientIntensity = u_material.ka;
	vec3 ambient = u_lightColor * ambientIntensity;
	
	vec3 diffuseInt = u_material.kd;
	float diffuseIntensity = max(dot(norm,lightDir), 0.0f);
	vec3 diffuse = diffuseInt * u_lightColor * diffuseIntensity;
	
	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));
	
	vec3 specularIntensity = u_material.ks * vec3(texSpecularColor);
	//vec3 cameraDir = normalize(camera + norm);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));

	float spec = pow(max(dot(cameraDir, reflectDir), 0.0f), u_material.sh * 128.0f);
	//float spec = sin(length(distance(vec3(v_position), lightPos)));
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = specularIntensity * spec * u_lightColor * facing;

	vec3 output = baseColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}



