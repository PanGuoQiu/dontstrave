#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D bgTexture;

void main()
{
	vec4 texColor = texture(bgTexture, TexCoords) * vec4(0.918f, 0.879f, 0.828f, 1.0f);
	if (texColor.a < 0.4)
		discard;

	FragColor = texColor;
}