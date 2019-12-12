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
discard: allows to ignore a pixel color output
*/

void main()
{
     vec4 texColor = texture(u_texture, v_textureCoord);
     color = texColor;
}