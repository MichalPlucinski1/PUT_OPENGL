#version 330
uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela


in vec4 n;
in vec4 l;
in vec4 v;

in vec2 iTexCoord0;

void main(void) {
	vec4 kd = texture(textureMap0, iTexCoord0);

	pixelColor = kd;

}