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
out vec3 v_lightDir;
out mat3 v_TBN;

void main()
{
	//CONVERTION FROM MODEL TO VIEW SPACE
	mat4 modelViewMat = u_model * u_view;
	v_position = modelViewMat * position;
	v_normal = normalize(vec3(modelViewMat * vec4(normal, 1.0f)));
	v_tangent = normalize(vec3(modelViewMat * vec4(tangent, 1.0f)));
	v_bitangent = normalize(vec3(modelViewMat * vec4(bitangent, 1.0f)));

	//CONVERTION FROM VIEW TO TANGENT SPACE
	mat3 TBN = transpose(mat3(v_tangent, v_bitangent, v_normal));
	v_TBN = TBN;
	v_cameraPos = vec3(modelViewMat * vec4(u_cameraPosition,1.0f));
	v_cameraPos = TBN * v_cameraPos;

	vec3 lightPos = vec3(modelViewMat * vec4(u_lightPosition,1.0f));
	v_lightDir = TBN * (lightPos - vec3(v_position));

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

uniform Material u_material;

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
in vec3 v_cameraPos;
//light
in vec3 v_lightDir;
in mat3 v_TBN;

void main()
{
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);
	vec4 texNormalColor = 2.0f * normalize(texture(u_texture_normal_1, v_textureCoord)) - 1.0f;
	texNormalColor = normalize(texNormalColor);
	vec3 baseColor = u_colorBase * texDiffuseColor.rgb;
	
	vec4 positionTS = v_position;
	vec3 normalTS = normalize(v_TBN * texNormalColor.rgb);

	vec3 cameraPositionTS = v_cameraPos;
	vec3 cameraDirectionTS = normalize(cameraPositionTS - vec3(positionTS));
	
	vec3 lightDirectionTS = normalize(v_lightDir);
	vec3 reflectDirectionTS = normalize(reflect(-lightDirectionTS,normalTS));

	//LIGHTING CALCULATIONS
	vec3 ambientIntensity = u_material.ka;
	vec3 ambient = u_lightColor * ambientIntensity;

	vec3 diffuseIntensity = u_material.kd * texDiffuseColor.rgb;
	float diffuseFact = max(dot(normalTS, lightDirectionTS), 0.0f);
	vec3 diffuse = diffuseFact * u_lightColor * diffuseIntensity;
	
	vec3 specularIntensity = u_material.ks * texSpecularColor.rgb;
	float specularFact = pow(max(dot(cameraDirectionTS, reflectDirectionTS), 0.0f), u_material.sh * 256.0f);
	float facing = dot(normalTS, lightDirectionTS) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = specularIntensity * specularFact * facing;

	vec3 output = baseColor * (ambient + diffuse) + specular;
	color = vec4(output, 1.0f);
}
