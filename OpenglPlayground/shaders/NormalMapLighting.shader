#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_mvp;

uniform vec3 u_cameraPosition;
uniform vec3 u_lightPosition;

out vec4 v_position;
out vec2 v_textureCoord;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
out vec3 v_cameraPos;
out vec3 v_lightPos;

void main()
{
	//CONVERTION FROM MODEL TO VIEW SPACE
	mat3 toModelMatrix = mat3(transpose(inverse(u_model)));
	v_normal = normalize(toModelMatrix * normal);
	v_tangent = normalize(toModelMatrix * tangent);
	v_bitangent = normalize(toModelMatrix * bitangent);

	//REO-ORTOGONALIZE TANGENT RESPECT TO NORMAL
	v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);


	//CONVERT TO RIGHT HAND SYSTEM
	if (dot(-cross(v_normal, v_tangent), v_bitangent) < 0.0f)
		v_tangent = -v_tangent;

	//CALCULATE TBN MATRIX
	mat3 TBN = transpose(mat3(v_tangent, v_bitangent, v_normal));

	//CONVERTION FROM MODEL TO TANGENT SPACE
	v_cameraPos = TBN * u_cameraPosition;
	v_lightPos = TBN * u_lightPosition;
	vec4 pos = u_model * position;
	v_position = vec4(TBN * pos.xyz,0.0f);
	
	//OPENGL SPECIFIC
	v_textureCoord = texCoord;
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
uniform sampler2D u_texture_normal_1;

uniform vec3 u_colorBase;
uniform vec3 u_lightColor;
uniform vec3 u_attConst;

//model 
in vec4 v_position;
in vec2 v_textureCoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;

in vec3 v_cameraPos;
in vec3 v_lightPos;

void main()
{
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);
	vec4 texNormalColor =  2.0f * texture(u_texture_normal_1, v_textureCoord) - 1.0f;
	vec3 normalTS = normalize(texNormalColor).rgb;
	normalTS.y = -normalTS.y;
	normalTS.z = -normalTS.z;

	vec3 cameraDirectionTS = normalize(v_cameraPos - v_position.xyz);
	vec3 lightDirectionTS = normalize(v_lightPos - v_position.xyz);
	vec3 halfwayTS = normalize(lightDirectionTS + cameraDirectionTS);

	float distance = length(v_lightPos - v_position.xyz);
	float att = 1.0f / (u_attConst.x + u_attConst.y * distance + u_attConst.z * (distance * distance));


	//LIGHTING CALCULATIONS
	vec3 emissiveColor = u_colorBase;

	vec3 ambientFact = u_meshMaterial.ka * texDiffuseColor.rgb;
	vec3 ambient = u_lightColor * ambientFact;
	ambient *= att;

	vec3 diffuseIntensity = u_meshMaterial.kd * texDiffuseColor.rgb;
	float diffuseFact = max(dot(normalTS, lightDirectionTS), 0.0f);
	vec3 diffuse = (diffuseFact * u_lightColor) * diffuseIntensity;
	diffuse *= att;

	vec3 specularFact = u_meshMaterial.ks * texSpecularColor.rgb;
	float specularIntensity = max(0.0f, sign(dot(normalTS, lightDirectionTS))) * pow(max(0.0f, dot(cameraDirectionTS, halfwayTS)), u_meshMaterial.sh);
	float facing = dot(normalTS, lightDirectionTS) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = (u_lightColor * specularFact * specularIntensity) * (diffuseIntensity * diffuseFact) * facing;
	specular *= att;

	vec3 output = emissiveColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}
