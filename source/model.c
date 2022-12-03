#include "model.h"
#include "obj_loader.h"

#include <assert.h> // assert()
#include <stddef.h> // offsetof()
#include <stdlib.h> // malloc(), free()

void model_init(model_t * model)
{
    mat4x4_init(&(model->transform), 1.0f);

    model->primitive_count = 0;
    model->primitive_list = NULL;

    model->material_count = 0;
    model->material_list = NULL;
}

void model_term(model_t * model)
{
    for (size_t i = 0; i < model->primitive_count; ++i) {
        primitive_t * primitive = &(model->primitive_list[i]);

        glDeleteVertexArrays(1, &(primitive->vao));
        glDeleteBuffers(1, &(primitive->vbo));
    }

    free(model->primitive_list);
    model->primitive_list = NULL;

    for (size_t i = 0; i < model->material_count; ++i) {
        material_term(&(model->material_list[i]));
    }
    
    free(model->material_list);
    model->material_list = NULL;
}

bool model_load(model_t * model, const char * filename)
{
    assert(model);
    
    mesh_t * mesh_list = NULL;
    size_t mesh_count = 0;

    if (!load_obj(filename, &mesh_list, &mesh_count, &model->material_list, &model->material_count)) {
        // TODO: Cleanup?
        return false;
    }

    model->primitive_count = mesh_count;
    model->primitive_list = (primitive_t *)malloc(sizeof(primitive_t) * mesh_count);

    for (size_t i = 0; i < mesh_count; ++i) {
        mesh_t * mesh = &mesh_list[i];
        primitive_t * primitive = &(model->primitive_list[i]);

        primitive->vertex_count = mesh->vertex_count;
        primitive->type = mesh->type;
        primitive->material = mesh->material;
        
        // Vertex Attribute Array (VAO), used for passing per-vertex inputs
        glGenVertexArrays(1, &(primitive->vao));
        glBindVertexArray(primitive->vao);

        // Packed Vertex Buffer Object (VBO), stores raw per-vertex input data
        glGenBuffers(1, &(primitive->vbo));

        // Upload our input data to the driver
        glBindBuffer(GL_ARRAY_BUFFER, primitive->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(vertex_t), mesh->vertex_list, GL_STATIC_DRAW);

        // Enable each vertex input
        glEnableVertexAttribArray(VERTEX_LOCATION_POSITION);
        glEnableVertexAttribArray(VERTEX_LOCATION_NORMAL);
        glEnableVertexAttribArray(VERTEX_LOCATION_TEXCOORD);
        
        // Configure each vertex input, with offsets into the packed vertex structure
        glVertexAttribPointer(VERTEX_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
        glVertexAttribPointer(VERTEX_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, normal));
        glVertexAttribPointer(VERTEX_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, texcoord));

        mesh_term(mesh);
    }

    free(mesh_list);

    return true;
}

void model_render(model_t * model, GLuint shader, mat4x4_t * view, mat4x4_t * projection)
{
    glUseProgram(shader);
    
    GLint mvp_location = glGetUniformLocation(shader, "u_MVP");

    // MVP = projection * view * model
    mat4x4_t mvp;
    mat4x4_xmul(&mvp, projection, view);
    mat4x4_mul(&mvp, &(model->transform));

    // Upload MVP data to the driver
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const float *)mvp.data);

    for (size_t i = 0; i < model->primitive_count; ++i) {
        primitive_t * primitive = &(model->primitive_list[i]);

        material_bind(primitive->material, shader);

        // Select the VAO we want to render from
        glBindVertexArray(primitive->vao);

        // Render N vertices from the selected VAO using the primitive type (usually GL_TRIANGLES)
        glDrawArrays(primitive->type, 0, primitive->vertex_count);
    }
}