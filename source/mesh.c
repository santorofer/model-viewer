#include "mesh.h"

#include <assert.h> // assert()
#include <stdlib.h> // malloc(), free()

void mesh_init(mesh_t * mesh)
{
    assert(mesh);

    mesh->name = NULL;

    mesh->type = GL_TRIANGLES;

    mesh->vertex_count = 0;
    mesh->vertex_list = NULL;

    mesh->material = NULL;
}

void mesh_term(mesh_t * mesh)
{
    if (!mesh) {
        return;
    }

    free(mesh->name);
    mesh->name = NULL;

    free(mesh->vertex_list);
    mesh->vertex_list = NULL;

    mesh->material = NULL;
}
