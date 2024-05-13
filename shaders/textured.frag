#version 410 core

in vec2 textureCoord;
in float zDepth;
in vec4 world_position;

uniform sampler2D rgbTexture;
//uniform sampler2D depthTexture;
uniform float SPELL_LIFE;
uniform float CAST_LIFE;
uniform vec3 SPELL_HEAD;
uniform vec3 SPELL_FOCUS;

out vec4 color;

subroutine vec4 spell (vec4 color);
subroutine uniform spell currentSpell;

subroutine (spell) vec4 emptySpell(vec4 color) {
    return color;
}
subroutine (spell) vec4 castSpell(vec4 color) {
    float spellDistance = length(cross(SPELL_FOCUS, SPELL_HEAD-world_position.xyz));
    return mix(vec4(CAST_LIFE, color.yzw), color, spellDistance);
}
subroutine (spell) vec4 releaseSpell(vec4 color) {
    float spellDistance = length(cross(SPELL_FOCUS, SPELL_HEAD-world_position.xyz));
    return mix(color, vec4(color.xy, SPELL_LIFE, 1.0), spellDistance);
}

void main(){
    color = texture(rgbTexture, textureCoord)/clamp(zDepth, 1.0, 10.0);
    color = currentSpell(color);
}
