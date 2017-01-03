#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 color;	// The color variable has attribute position 1
layout (location = 2) in vec2 texCoord;

out vec3 ourColor; // Output a color to the fragment shader
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = color; // Set ourColor to the input color we got from the vertex data
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}       