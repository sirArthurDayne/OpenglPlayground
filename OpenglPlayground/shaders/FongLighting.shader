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
	
	//SMOOTH SHADING
	//v_normal = mat3(transpose(inverse(u_model))) * vec3(normalize(position));
	
	//FLAT SHADING
	v_normal = mat3(transpose(inverse(u_model))) * normalize(normal);
	
	//OPENGL SPECIFIC
	v_textureCoord = textureCoord;
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
uniform Material u_material;//user
uniform Material u_meshMaterial;//model

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

	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));
	
	vec3 lightPos = u_lightPosition;
	vec3 lightDir = normalize(lightPos - vec3(v_position));
	
	vec3 halfwayVec = normalize(lightDir + cameraDir);

	//LIGHT CALCULATIONS
	vec3 emissiveColor = u_colorBase;
	
	vec3 ambientFact = u_meshMaterial.ka * texDiffuseColor.rgb;
	vec3 ambient = u_lightColor * ambientFact;
	
	vec3 diffuseFact = u_meshMaterial.kd * texDiffuseColor.rgb;
	float diffuseIntensity = max(dot(norm,lightDir), 0.0f);
	vec3 diffuse = (diffuseFact * u_lightColor) * diffuseIntensity;
	
	vec3 specularFact = u_meshMaterial.ks * texSpecularColor.rgb;
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	//float spec = pow(max(dot(cameraDir, halfwayVec),0.0f), u_material.sh * 128.0f);
	float spec = max(0.0f, sign(dot(norm, lightDir))) * pow(max(0.0f, dot(cameraDir,halfwayVec)), u_meshMaterial.sh);
	vec3 specular = (specularFact* spec) * u_lightColor * facing;

	vec3 output = emissiveColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}



