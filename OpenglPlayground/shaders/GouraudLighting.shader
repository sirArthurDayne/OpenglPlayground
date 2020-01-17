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
uniform Material u_meshMaterial;

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
	//OPENGL SPECIFIC
	gl_Position = u_mvp * position;
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, textureCoord);


	vec4 v_position = u_model * position;


	//smooth shading
	//vec3 v_normal = mat3(transpose(inverse(u_model))) * normalize(position).xyz;
	//flat shading
	vec3 v_normal = mat3(transpose(inverse(u_model))) * normalize(normal);
	v_normal = normalize(v_normal);
	
	//calculate camera
	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));

	vec3 lightDir = normalize(u_lightPosition - vec3(v_position));
	vec3 halfwayVec = normalize(lightDir + cameraDir);
	
	//LIGHTING CALCULATIONS
	vec3 emissiveColor = u_colorBase;

	vec3 ambientFact = u_meshMaterial.ka * texDiffuseColor.rgb;
	vec3 ambient = ambientFact * u_lightColor;
	
	vec3 diffuseFact = u_meshMaterial.kd * texDiffuseColor.rgb;
	float diffuseIntensity = max(dot(v_normal, lightDir), 0.0f);
	vec3 diffuse = (diffuseFact * u_lightColor) * diffuseIntensity;

	vec3 specularFact = u_meshMaterial.ks * texSpecularColor.rgb;
	float spec = max(0.0f, sign(dot(v_normal, lightDir))) * pow(max(0.0f, dot(cameraDir,halfwayVec)), u_meshMaterial.sh);
	float facing = dot(v_normal, lightDir) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = (specularFact * spec * u_lightColor) * facing;

	v_outputColor = emissiveColor * (ambient + diffuse + specular);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_outputColor;

void main(void)
{
	color = vec4(v_outputColor, 1.0f);
}
