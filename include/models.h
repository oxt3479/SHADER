#ifndef MODELS_H
#define MODELS_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "shaderObjects.h"

std::vector<VAO> load_model(const char * objFile);

#endif