#version 330


out vec4 pixelColor;

in vec2 iTexCoord;

uniform sampler2D Texture;

void main(void) {
	vec4 kd = texture(Texture, iTexCoord);

	pixelColor = vec4(kd.rgb, kd.a);
}