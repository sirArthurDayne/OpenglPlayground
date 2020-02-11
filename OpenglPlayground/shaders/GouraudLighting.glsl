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
uniform float u_hasMaterials;

uniform sampler2D u_texture_diffuse_1;
uniform sampler2D u_texture_specular_1;

uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_colorBase;
uniform vec3 u_cameraPosition;
uniform vec3 u_attConst;//x:kc, y:kl, z:kq

uniform mat4 u_model;
uniform mat4 u_mvp;
uniform mat4 u_view;

out vec3 v_outputColor;

vec3 BlinPhong(vec3 normal, vec3 diffuseColor, vec3 specularColor, vec3 cameraDirection, 
	vec3 lightDirection, vec3 halfway, vec3 lightColor, Material mat, float attenuation)
{
	vec3 emissiveColor = u_colorBase;
	
	vec3 ambientFact = mat.ka * diffuseColor;
	vec3 ambient = lightColor * ambientFact;
	ambient *= attenuation;

	vec3 diffuseFact = mat.kd * diffuseColor;
	float diffuseIntensity = max(dot(normal,lightDirection), 0.0f);
	vec3 diffuse = (diffuseFact * lightColor) * diffuseIntensity;
	diffuse *= attenuation;

	vec3 specularFact = mat.ks * specularColor;
	float facing = dot(normal, lightDirection) > 0.0f ? 1.0f : 0.0f;
	float spec = max(0.0f, sign(dot(normal, lightDirection))) * pow(max(0.0f, dot(cameraDirection, halfway)), mat.sh);
	vec3 specular = (specularFact* spec * lightColor) *(diffuseFact * diffuseIntensity) * facing;
	specular *= attenuation;

	return  emissiveColor * (ambient + diffuse + specular);
}





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
	
	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - v_position.xyz);

	vec3 lightDir = normalize(u_lightPosition - v_position.xyz);
	vec3 halfwayVec = normalize(lightDir + cameraDir);
	
	
	float distance = length(u_lightPosition - v_position.xyz);
	float att = 1.0f / (u_attConst.x + u_attConst.y * distance + u_attConst.z * (distance * distance));


	//LIGHTING CALCULATIONS
	if (u_hasMaterials == 2.0f)//textures and materials
	{
		v_outputColor = BlinPhong(v_normal, texDiffuseColor.rgb, texSpecularColor.rgb,
				cameraDir, lightDir, halfwayVec, u_lightColor, u_meshMaterial, att);
	}
	else if (u_hasMaterials == 1.0f)//only textures
	{
		v_outputColor = BlinPhong(v_normal, texDiffuseColor.rgb, texSpecularColor.rgb,
				cameraDir, lightDir, halfwayVec, u_lightColor, u_material, att);
	}
	else//nothing, just the model data
	{
		Material material = u_material;
		material.sh *= 256.0f;
		v_outputColor = BlinPhong(v_normal, vec3(1.0f), vec3(1.0f),
				cameraDir, lightDir, halfwayVec, u_lightColor, material, att);
	}
		

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_outputColor;

void main(void)
{
	color = vec4(v_outputColor, 1.0f);
}
