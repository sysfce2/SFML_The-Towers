#version 330 core

// Ins
in vec2 pass_uv;

// Outs
out vec4 out_color;

// Uniforms
uniform sampler2D ourTexture;

void main(){
	out_color = texture(ourTexture, pass_uv);
}
