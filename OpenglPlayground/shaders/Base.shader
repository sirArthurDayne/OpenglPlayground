#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;

uniform mat4 u_mvp;
/*varying: allows to send data from Vertex to Fragment Shader*/
out vec2 v_textureCoord;
out vec4 v_position;
void main()
{
  gl_Position = u_mvp * position;
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

void main()
{
     vec4 texColor = texture(u_texture, v_textureCoord);
     color = texColor;
}