#version 410 core

in vec3 fragmentColor;
in vec2 textureCoord;
in float zDepth;

uniform sampler2D textureA;

out vec4 color;

void main(){
    color = texture(textureA, textureCoord)/clamp(zDepth, 1.0, 100.0);
}
