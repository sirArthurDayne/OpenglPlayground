#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;
/*varying: allows to send data from Vertex to Fragment Shader*/
out vec2 v_textureCoord;
out vec4 v_position;
void main()
{
  gl_Position = position;
  v_textureCoord = textureCoord;  
  v_position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

/*uniforms: allous to change shader code in
the CPU side*/

uniform vec4 u_colorBase;
uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

uniform sampler2D u_texture;
in vec2 v_textureCoord;
in vec4 v_position;
/*
max: can be use for finding intersection
min: can be use for finding union
mod: allous to make recursive solution
polar cood: make for simplify calculations. vector becomes: v = (magnitude, angle)
*/

#define PI 3.141592
#define TAU (2.0f*PI)


vec2 ConvertToPolar(vec2 rectCoord)
{
	//vectors becomes: magnitude, angle
	return vec2(length(rectCoord), atan(rectCoord.y, rectCoord.x));
}

vec2 ConvertToRect(vec2 polarCoord)
{
	return vec2(polarCoord.x * cos(polarCoord.y), polarCoord.x * sin(polarCoord.y));
}


// Repeat in two dimensions
vec2 pMod2(inout vec2 p, vec2 size) 
{
	vec2 c = floor((p + size * 0.5) / size);
	p = mod(p + size * 0.5, size) - size * 0.5;
	return c;
}


float DistanceCircle(float radius, vec2 origin)
{
	return length(origin) - radius;
}

float DistanceRoundedBox(float radius, vec2 origin)
{
	/*the more pow increment the less rounded the box is*/
	origin = origin * origin;
	origin = origin * origin;
	float d8 = dot(origin, origin);
	return pow(d8, 1.0 / 8.0f) - radius;
}

float DistanceBox(vec2 size, vec2 origin)
{
	origin = abs(origin);
	float dx = 2.0f*origin.x - size.x;
	float dy = 2.0f*origin.y - size.y;
	return max(dx, dy);
}

float DrawMotionOne(vec2 position)
{
	position = abs(position);
	vec2 p_position = ConvertToPolar(position);
	p_position.y += position.x * sin(u_time);//rotation
	p_position.x *= 1.0f + position.y;//mod radius
	position = ConvertToRect(p_position);
	float distanceThree = DistanceBox(vec2(0.5f, 0.5f), position + vec2(0.0));
	return distanceThree;
}

float DrawCaleidoscope(vec2 position, float times)
{
	vec2 p_pos = ConvertToPolar(position);
	p_pos.y = mod(p_pos.y, TAU / times);
	p_pos.y += u_time;
	position = ConvertToRect(p_pos);
	pMod2(position, vec2(0.5));
	float d1 = DistanceCircle(0.2, position);
	float d2 = DistanceBox(vec2(0.2), position - vec2(0.1));
	return min(d1, d2);
}


void main()
{
	vec4 texColor = texture(u_texture, v_textureCoord);
	vec4 outputColor = vec4(0.0f);
	//setup scaling and origin pos
	vec2 uv = 2.0f* v_textureCoord.xy - 1.0f;
	uv.x *= u_resolution.x / u_resolution.y;
	//uv.y *= u_resolution.x / u_resolution.y;
	

	//float distance = DrawMotionOne(uv) + DistanceCircle(0.1f, vec2(0.4f, 0.2f));
	float distance = DrawCaleidoscope(uv, 9);
	float md = mod(distance, 0.1f);
	float nd = abs(distance / 0.3f) ;

	if (abs(distance) < 0.1f)
	{
		outputColor = vec4(0.5f);
		//discard;
	}
	if (abs(md) < 0.01f)
	{
		if (distance < 0.0f)
		outputColor = vec4(1.0f, 0.0f, 0.0f, 1.0f)/nd;
		else
		outputColor = vec4(0.0f, 1.0f, 0.0f, 1.0f)/nd;
	}
	color = outputColor;	
}

