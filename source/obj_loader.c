#include "obj_loader.h"
#include "texture.h"

#include <stdio.h> // FILE, fopen(), getline(), sscanf()
#include <assert.h> // assert()
#include <string.h> // strdup(), strlen(), strtok()
#include <stdlib.h> // malloc(), free()

bool load_obj(
    const char * filename,
    mesh_t ** mesh_list, size_t * mesh_count,
    material_t ** material_list, size_t * material_count
)
{
    assert(filename);
    assert(mesh_list);
    assert(mesh_count);
    assert(material_list);
    assert(material_count);
    
    // TODO: Fernando

    return true;
}