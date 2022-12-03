#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "mesh.h"
#include "material.h"

#include <stdbool.h>
#include <stdlib.h>

bool load_obj(
    const char * filename,
    mesh_t ** mesh_list, size_t * mesh_count,
    material_t ** material_list, size_t * material_count
);

#endif // OBJ_LOADER_H