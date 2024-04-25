#version 410 core

in vec2 textureCoord;
in float zDepth;

uniform sampler2D rgbTexture;
uniform sampler2D depthTexture;

out vec4 color;

void main(){
    color = texture(rgbTexture, textureCoord)/clamp(zDepth, 1.0, 10.0);
}
