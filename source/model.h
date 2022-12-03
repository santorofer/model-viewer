#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "vertex.h"

#include <glad/glad.h>
#include <glmm/glmm.h>

#include <stdbool.h>

typedef struct
{
    GLuint vao;
    GLuint vbo;

    GLenum type;

    size_t vertex_count;

    material_t * material;

} primitive_t;

typedef struct
{
    mat4x4_t transform;

    size_t primitive_count;

    primitive_t * primitive_list;

    size_t material_count;
    
    material_t * material_list;

} model_t;

void model_init(model_t * model);

void model_term(model_t * model);

bool model_load(model_t * model, const char * filename);

void model_render(model_t * model, GLuint shader, mat4x4_t * view, mat4x4_t * projection);

#endif // MODEL_H