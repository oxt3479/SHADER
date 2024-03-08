#include "models.h"

/*
Reference tutorial:
https://sibras.github.io/OpenGL4-Tutorials/docs/Tutorials/05-Tutorial5/
*/

std::vector<VAO> load_model(const char * objFile) {

    std::vector<VAO> all_vaos;

    const aiScene * p_Scene = aiImportFile( objFile,
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType);
 
    if (!p_Scene) {
        std::cout << "Error loading file " << aiGetErrorString() << std::endl;
        return all_vaos;
    }

    for (unsigned i = 0; i < p_Scene->mNumMeshes; i++) {

        const aiMesh * p_AIMesh = p_Scene->mMeshes[i];
            
        // Create the new mesh data
        const unsigned modelSizeVertices = p_AIMesh->mNumVertices * 8 * sizeof(GLfloat);
        const unsigned modelSizeIndices  = p_AIMesh->mNumFaces * 3 * sizeof(GLuint);

        GLfloat * vbo_head = (GLfloat *)malloc(modelSizeVertices);
        GLuint * ebo_head = (GLuint *)malloc(modelSizeIndices);
        
        // retain head pointer, by using alternative for assignment...
        GLfloat * vbo_ptr = vbo_head;
        GLuint * ebo_ptr = ebo_head;

        for (unsigned j = 0; j < p_AIMesh->mNumVertices; j++) {
            *vbo_ptr++ = (GLfloat) p_AIMesh->mVertices[j].x;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mVertices[j].y;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mVertices[j].z;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mNormals[j].x;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mNormals[j].y;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mNormals[j].z;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mTextureCoords[0][j].x;
            *vbo_ptr++ = (GLfloat) p_AIMesh->mTextureCoords[0][j].y;
        }
        for (unsigned j = 0; j < p_AIMesh->mNumFaces; j++) {
            *ebo_ptr++ = (GLuint) p_AIMesh->mFaces[j].mIndices[0];
            *ebo_ptr++ = (GLuint) p_AIMesh->mFaces[j].mIndices[1];
            *ebo_ptr++ = (GLuint) p_AIMesh->mFaces[j].mIndices[2];
        }
        
        VAO vao((GLfloat*) vbo_head, (GLsizeiptr) modelSizeVertices,
                (GLuint*)  ebo_head, (GLsizeiptr) modelSizeIndices);
        
        all_vaos.push_back(vao);
        
        free(vbo_head);
        free(ebo_head);
    }
    return all_vaos;
}
