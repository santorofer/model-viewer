
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <glmm/glmm.h>
#include <glad/glad.h>

#include "model.h"
#include "shaders.h"

int main(int argc, char ** argv)
{
    int width = 1024;
    int height = 768;

    SDL_Window * window = NULL;
    SDL_GLContext context = NULL;

    window = SDL_CreateWindow(
        "model-viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_OPENGL
    );

    // Maximum OpenGL version on OSX is 4.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);

    context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        return 1;
    }

    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);

    vec3f_t camera = {{ 10.0f, 10.0f, 10.0f }};
    vec3f_t focus = {{ 0.0f, 0.0f, 0.0f }};
    vec3f_t up = {{ 0.0f, 1.0f, 0.0f }};
    vec3f_t light = {{ 10.0f, 10.0f, 10.0f }};

    mat4x4_t model;
    mat4x4_init(&model, 1.0f);

    mat4x4_t view;
    mat4x4_init(&view, 1.0f);

    mat4x4_t projection;
    mat4x4_init(&projection, 1.0f);

    // load_obj()

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.type == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glmm_look_at(&view, &camera, &focus, &up);

        float aspect = (float)width / (float)height;
        glmm_perspective(&projection, aspect, 0.0001f, 1000.0f, 45.0f);

        mat4x4_t mvp; // MVP = projection * view * model
        mat4x4_xmul(&mvp, &projection, &view);
        mat4x4_mul(&mvp, &model);

        // render

        SDL_GL_SwapWindow(window);
    }

    return 0;
}