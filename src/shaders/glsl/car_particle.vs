#version 330 core
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec2 textCoords;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 0.02f;
    TexCoords = textCoords;
    ParticleColor = color;
    gl_Position = projection * view * vec4(pos * scale + offset, 1.0);
	//gl_Position = vec4(pos + offset, 1.0);
	
}