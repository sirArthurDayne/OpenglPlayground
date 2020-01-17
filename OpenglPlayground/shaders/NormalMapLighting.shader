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
out vec3 v_cameraDir;
out vec3 v_lightDir;
out vec3 v_halfwayDir;

void main()
{

	//CONVERTION FROM MODEL TO VIEW SPACE
	mat4 modelViewMat = u_model * u_view;
	v_position = modelViewMat * position;
	
	v_normal = mat3(transpose(inverse(modelViewMat))) * normal;
	v_normal = normalize(v_normal);
	
	v_tangent = normalize(vec3(modelViewMat * vec4(tangent,0.0f)));
	//REO-ORTOGONALIZE TANGENT RESPECT TO NORMAL
	v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);

	//CALCULATE BITANGENT AND TBN MATRIX
	v_bitangent = normalize(-cross(v_normal, v_tangent));

	//CONVERT TO RIGHT HAND SYSTEM
	if (dot(-cross(v_normal, v_tangent), v_bitangent) < 0.0f)
	{
		v_tangent = -v_tangent;
		//v_bitangent = normalize(-cross(v_normal, v_tangent));
	}

	mat3 TBN = transpose(mat3(
		v_tangent.x, v_bitangent.x, v_normal.x,
		v_tangent.y, v_bitangent.y, v_normal.y,
		v_tangent.z, v_bitangent.z, v_normal.z
	));

	//CONVERTION FROM VIEW TO TANGENT SPACE
	vec3 cameraPos = vec3(modelViewMat * vec4(u_cameraPosition,0.0f));
	v_cameraDir = TBN * (cameraPos - v_position.xyz);
	v_cameraDir = normalize(v_cameraDir);

	vec3 lightPos = vec3(modelViewMat * vec4(u_lightPosition,0.0f));
	v_lightDir = TBN * (lightPos - v_position.xyz);
	v_lightDir = normalize(v_lightDir);

	v_halfwayDir = normalize(v_lightDir + v_cameraDir);

	v_position = vec4(TBN * v_position.xyz, 0.0f);
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

//model 
in vec4 v_position;
in vec2 v_textureCoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
//camera
in vec3 v_cameraDir;
//light
in vec3 v_lightDir;
in vec3 v_halfwayDir;

void main()
{
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);
	
	vec4 texNormalColor = 2.0f * normalize(texture(u_texture_normal_1, v_textureCoord)) - 1.0f;
	vec3 normalTS = texNormalColor.rgb;

	vec3 cameraDirectionTS = normalize(v_cameraDir);
	vec3 lightDirectionTS = normalize(v_lightDir);

	vec3 halfwayTS = normalize(v_halfwayDir);
	
	//LIGHTING CALCULATIONS
	vec3 emissiveColor = u_colorBase;

	vec3 ambientFact = u_meshMaterial.ka * texDiffuseColor.rgb;
	vec3 ambient = u_lightColor * ambientFact;

	vec3 diffuseIntensity = u_meshMaterial.kd * texDiffuseColor.rgb;
	float diffuseFact = max(dot(normalTS, lightDirectionTS), 0.0f);
	vec3 diffuse = (diffuseFact * u_lightColor) * diffuseIntensity;
	
	vec3 specularFact = u_meshMaterial.ks * texSpecularColor.rgb;
	float specularIntensity = pow(max(dot(cameraDirectionTS, halfwayTS), 0.0f), u_meshMaterial.sh);
	float facing = dot(normalTS, lightDirectionTS) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = u_lightColor * (specularFact * specularIntensity) * facing;

	vec3 output = emissiveColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}
