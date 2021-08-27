#version 330 core
out vec4 diffuse;

uniform sampler2D map;

in vec3 _color;


void main() {
	diffuse = vec4(_color.rgb, 1);
}
