#version 330 core

out vec4 out_Color;

in vec3 fragColor;

void main(){
	out_Color=vec4(fragColor, 1.0);
}