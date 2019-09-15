#version 330 core

in vec2 uvs;
out vec4 fragColor;

uniform sampler2D fboTex;

void main(void)
{
	vec2 uvFlip = uvs;
	uvFlip.y = 1.0 - uvFlip.y;
    vec4 fbo = texture(fboTex, uvFlip);
    fragColor = fbo;
}
