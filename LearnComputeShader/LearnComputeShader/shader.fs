#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

// texture samplers
uniform sampler3D texture1;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture1, TexCoord);
}
