#include "config.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <stdexcept>
#include <array>

struct TextureLibrary {
    const char* pentagon_paths[3] = {
        TEXTURE_DIR "/curved.png",
        TEXTURE_DIR "/tile_floor_b.png",
        TEXTURE_DIR "/tile_floor_a.png"
    };
    const char* specular_paths[3] = {
        TEXTURE_DIR "/curved_spec.png",
        TEXTURE_DIR "/tile_floor_b_disp.png",
        TEXTURE_DIR "/tile_floor_a.png"
    };
    const char* grimoire_paths[2] = {
        TEXTURE_DIR "/grimoire_cover.png",
        TEXTURE_DIR "/grimoire_page_1.png"
    };
    void linkPentagonLibrary(GLuint shaderID);
    void linkGrimoireLibrary(GLuint shaderID);
    
    void readRGBATextureArray(const char* paths[], int num_imgs, int prog_index);
};