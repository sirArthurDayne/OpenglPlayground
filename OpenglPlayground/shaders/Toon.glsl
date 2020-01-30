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
uniform float u_hasMaterials;

uniform sampler2D u_texture_diffuse_1;
uniform sampler2D u_texture_specular_1;

uniform vec3 u_colorBase;
uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec3 u_cameraPosition;
uniform vec3 u_attConst;//x:kc, y:kl, z:kq

in vec2 v_textureCoord;
in vec4 v_position;
in vec3 v_normal;



float EdgeDetection(vec3 viewDirection, vec3 normal, float limit)
{
	return (dot(viewDirection, normal) > limit) ? 1.0f : 0.0f;
}



vec3 BlinPhong(vec3 normal, vec3 diffuseColor, vec3 specularColor, vec3 cameraDirection, 
	vec3 lightDirection, vec3 halfway, vec3 lightColor, Material mat, float attenuation)
{
	//apply edge detection algorithm
	vec3 emissiveColor = u_colorBase * EdgeDetection(cameraDirection, normal, 0.25f);
	
	vec3 ambientFact = mat.ka * diffuseColor;
	vec3 ambient = lightColor * ambientFact;
	ambient *= attenuation;

	//toon effect diffuse
	const int colorGrad = 5;

	vec3 diffuseFact = mat.kd * diffuseColor;
	float diffuseIntensity = max(dot(normal,lightDirection), 0.0f);
	vec3 diffuse = (diffuseFact * lightColor) * floor(diffuseIntensity * colorGrad);
	diffuse *= attenuation;

	//apply specular toon effect
	vec3 specularFact = mat.ks * specularColor;
	//float spec =  dot(lightDirection, normal) > 0.0f ? pow(max(0.0f, dot(halfway, normal)), mat.sh) : 0.0f;
	float spec = max(0.0f, sign(dot(normal, lightDirection))) * pow(max(0.0f, dot(cameraDirection, halfway)), mat.sh);
	float specularMask = (pow(dot(halfway, normal), mat.sh) > 0.4f) ? 1.0f : 0.f;

	vec3 specular = (specularFact* spec * lightColor) *(diffuseFact * diffuseIntensity) * specularMask;
	specular *= attenuation;

	return  emissiveColor * (ambient + diffuse + specular);
}
void main()
{
	vec3 norm = normalize(v_normal);
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);

	vec3 camera = u_cameraPosition;
	vec3 cameraDir = normalize(camera - vec3(v_position));
	
	vec3 lightPos = u_lightPosition;
	vec3 lightDir = normalize(lightPos - vec3(v_position));

	float distance = length(lightPos - v_position.xyz);
	float att = 1.0f / (u_attConst.x + u_attConst.y * distance + u_attConst.z * (distance * distance));
	
	vec3 halfwayVec = normalize(lightDir + cameraDir);

	//LIGHTING CALCULATIONS
	vec3 output = vec3(0.0f);
	if (u_hasMaterials == 2.0f)//textures and materials
	{
		output = BlinPhong(norm, texDiffuseColor.rgb, texSpecularColor.rgb,
				cameraDir, lightDir, halfwayVec, u_lightColor, u_meshMaterial, att);
	}
	else if (u_hasMaterials == 1.0f)//only textures
	{
		output = BlinPhong(norm, texDiffuseColor.rgb, texSpecularColor.rgb,
				cameraDir, lightDir, halfwayVec, u_lightColor, u_material, att);
	}
	else//nothing, just the model data
	{
		output = BlinPhong(norm, vec3(1.0f), vec3(1.0f),
				cameraDir, lightDir, halfwayVec, u_lightColor, u_material, att);
	}
	
	color = vec4(output, 1.0f);
}
