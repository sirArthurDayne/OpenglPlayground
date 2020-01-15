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

void main()
{
	//CONVERTION FROM MODEL TO VIEW SPACE
	mat4 modelViewMat = u_model * u_view;
	v_position = modelViewMat * position;
	v_normal = normalize(vec3(modelViewMat * vec4(normal,0.0f)));
	v_tangent = normalize(vec3(modelViewMat * vec4(tangent,0.0f)));

	//REO-ORTOGONALIZE TANGENT RESPECT TO NORMAL
	v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);

	//CALCULATE BITANGENT AND TBN MATRIX
	v_bitangent = normalize(-cross(v_normal, v_tangent));

	//Conversion to right handed
	if (dot(-cross(v_normal, v_tangent), v_bitangent) < 0.0f)
		v_tangent = -v_tangent;

	mat3 TBN = transpose(mat3(v_tangent, v_bitangent, v_normal));
	//CONVERTION FROM VIEW TO TANGENT SPACE
	vec3 cameraPos = vec3(modelViewMat * vec4(u_cameraPosition,1.0f));
	v_cameraDir = TBN * normalize(cameraPos - vec3(v_position));

	vec3 lightPos = vec3(modelViewMat * vec4(u_lightPosition,1.0f));
	v_lightDir = TBN * normalize(lightPos - vec3(v_position));

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
in vec3 v_cameraDir;
//light
in vec3 v_lightDir;

void main()
{
	vec4 texDiffuseColor = texture(u_texture_diffuse_1, v_textureCoord);
	vec4 texSpecularColor = texture(u_texture_specular_1, v_textureCoord);
	vec4 texNormalColor = 2.0f * texture(u_texture_normal_1, v_textureCoord) - 1.0f;
	texNormalColor = normalize(texNormalColor);
	
	vec3 emissiveColor = u_colorBase * texDiffuseColor.rgb;
	
	vec3 normalTS = texNormalColor.rgb;

	vec3 cameraDirectionTS = normalize(v_cameraDir);
	vec3 lightDirectionTS = normalize(v_lightDir);

	//vec3 reflectDirectionTS = normalize(reflect(-lightDirectionTS,normalTS));
	vec3 halfwayTS = normalize(lightDirectionTS + cameraDirectionTS);
	//LIGHTING CALCULATIONS
	vec3 ambientIntensity = u_material.ka;
	vec3 ambient = u_lightColor * ambientIntensity;

	vec3 diffuseIntensity = u_material.kd;
	float diffuseFact = max(dot(normalTS, lightDirectionTS), 0.0f);
	vec3 diffuse = (diffuseFact * u_lightColor) * diffuseIntensity;
	
	vec3 specularIntensity = u_material.ks * texSpecularColor.rgb;
	float specularFact = pow(max(dot(cameraDirectionTS, halfwayTS), 0.0f), u_material.sh * 128.0f);
	float facing = dot(normalTS, lightDirectionTS) > 0.0f ? 1.0f : 0.0f;
	vec3 specular = (u_lightColor * specularFact) * specularIntensity * facing;

	vec3 output = emissiveColor * (ambient + diffuse) + specular;
	//vec3 output = v_tangent;
	color = vec4(output, 1.0f);
}
