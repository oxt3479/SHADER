#version 410 core

layout(location = 0) in vec3 model_verts;
layout(location = 1) in vec2 text_coords;
layout(location = 2) in mat4 DISP;

uniform sampler2D depthTexture;
uniform mat4 CAMERA;
uniform mat4 WORLD;
uniform float u_time;

out vec2 textureCoord;
out float zDepth;

void main(){
    vec4 world_Pos = DISP * vec4(model_verts, 1.0); // Cell DISPlacement
    mat3 REFLECT_ONLY = mat3(DISP); // Remove the displacment, keeping only the reflection...
    vec3 normal = normalize(REFLECT_ONLY * model_verts);

    textureCoord = text_coords;
    
    world_Pos -= vec4(normal * (
        texture(depthTexture, textureCoord).g
        ), 0.0);

    world_Pos = WORLD * world_Pos; // Player movement in WORLD
    gl_Position = CAMERA * world_Pos; // Player camera
    zDepth = gl_Position.z;    
}