#version 410 core

layout(location = 0) in vec3 model_verts;
layout(location = 1) in mat4 DISP;

uniform mat4 CAMERA;
uniform mat4 WORLD;
uniform float u_time;

#define STEREO_OFFSET 5.0f

out vec3 fragmentColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void applyStereoGraphic(inout vec4 wPos) {
    wPos = vec4(    -wPos.x/(wPos.z-STEREO_OFFSET)*STEREO_OFFSET,
                    -wPos.y/(wPos.z-STEREO_OFFSET)*STEREO_OFFSET,
                    length(wPos-vec4(0.,0.,STEREO_OFFSET,0.))-STEREO_OFFSET,
                    1.0);
}

void main(){
    vec4 world_Pos = DISP * vec4(model_verts, 1.0); // Cell DISPlacement
    fragmentColor = vec3(rand(world_Pos.xy), rand(world_Pos.xz), rand(world_Pos.yz));
    world_Pos = WORLD * world_Pos; // Player movement in WORLD
    applyStereoGraphic(world_Pos); // Apply stereogaphic warp.
    gl_Position = CAMERA * world_Pos; // Player camera
}