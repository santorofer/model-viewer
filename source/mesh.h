#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "material.h"

#include <stdbool.h>

#include <glad/glad.h>

typedef struct
{
    char * name;

    GLenum type;

    unsigned int vertex_count;

    vertex_t * vertex_list;

    material_t * material;

} mesh_t;

void mesh_init(mesh_t * mesh);

void mesh_term(mesh_t * mesh);

#endif // MESH_H