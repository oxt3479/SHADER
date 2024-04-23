#version 410 core

in vec3 fragmentColor;
in vec2 textureCoord;
in float zDepth;

uniform sampler2D rgbTexture;
uniform sampler2D depthTexture;

out vec4 color;

void main(){
    float depth = texture(depthTexture, textureCoord).r;
    color = texture(rgbTexture, textureCoord)/clamp(zDepth/depth, 1.0, 100.0);
}
