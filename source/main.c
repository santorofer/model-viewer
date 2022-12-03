
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <glmm/glmm.h>
#include <glad/glad.h>

#include "model.h"
#include "shader.h"

// OpenGL Shading Language (GLSL)
// https://en.wikipedia.org/wiki/OpenGL_Shading_Language

const char * VERTEX_GLSL = 
    "#version 410 core                                                                      \n"
    "                                                                                       \n"
    "uniform mat4 u_MVP;                                                                    \n"
    "                                                                                       \n"
    "layout(location = 0) in vec3 a_Position;                                               \n"
    "layout(location = 1) in vec3 a_Normal;                                                 \n"
    "layout(location = 2) in vec2 a_TexCoord;                                               \n"
    "                                                                                       \n"
    "out vec3 v_Position;                                                                   \n"
    "out vec3 v_Normal;                                                                     \n"
    "out vec2 v_TexCoord;                                                                   \n"
    "                                                                                       \n"
    "void main() {                                                                          \n"
    "  gl_Position = u_MVP * vec4(a_Position, 1.0);                                         \n"
    "  v_Position = a_Position;                                                             \n"
    "  v_Normal = normalize(a_Normal);                                                      \n"
    "  v_TexCoord = a_TexCoord;                                                             \n"
    "}                                                                                      \n"
;

const char * FRAGMENT_GLSL = 
    "#version 410 core                                                                      \n"
    "                                                                                       \n"
    "vec4 u_Ambient;                                                                        \n"
    "vec4 u_Diffuse;                                                                        \n"
    "vec4 u_Specular;                                                                       \n"
    "float u_Shininess;                                                                     \n"
    "float u_Dissolve;                                                                      \n"
    "                                                                                       \n"
    "uniform sampler2D u_AmbientMap;                                                        \n"
    "uniform sampler2D u_DiffuseMap;                                                        \n"
    "uniform sampler2D u_SpecularMap;                                                       \n"
    "uniform sampler2D u_BumpMap;                                                           \n"
    "uniform sampler2D u_ReflectionMap;                                                     \n"
    "                                                                                       \n"
    "uniform vec3 u_CameraPos;                                                              \n"
    "uniform vec3 u_LightPos;                                                               \n"
    "                                                                                       \n"
    "in vec3 v_Position;                                                                    \n"
    "in vec3 v_Normal;                                                                      \n"
    "in vec2 v_TexCoord;                                                                    \n"
    "                                                                                       \n"
    "layout(location = 0) out vec4 o_Color;                                                 \n"
    "                                                                                       \n"
    "void main() {                                                                          \n"
    "  vec3 color = normalize(u_Diffuse.rgb + texture(u_DiffuseMap, v_TexCoord).rgb);       \n"
    "                                                                                       \n"
    "  vec3 ambient = 0.05 * color;                                                         \n"
    "                                                                                       \n"
    "  vec3 lightDir = normalize(u_LightPos - v_Position);                                  \n"
    "  float diff = max(dot(lightDir, v_Normal), 0.0);                                      \n"
    "  vec3 diffuse = diff * color;                                                         \n"
    "                                                                                       \n"
    "  vec3 viewDir = normalize(u_CameraPos - v_Position);                                  \n"
    "  vec3 reflectDir = reflect(-lightDir, v_Normal);                                      \n"
    "  vec3 halfwayDir = normalize(lightDir + viewDir);                                     \n"
    "  float spec = pow(max(dot(v_Normal, halfwayDir), 0.0), 32.0);                         \n"
    "  vec3 specular = vec3(0.3) * spec;                                                    \n"
    "                                                                                       \n"
    "  o_Color = vec4(ambient + diffuse + specular, 1.0);                                   \n"
    "}                                                                                      \n"
;

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

    printf("OpenGL Version: %s\n",  glGetString(GL_VERSION));
    printf("GLSL Version: %s\n",    glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("OpenGL Vendor: %s\n",   glGetString(GL_VENDOR));
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);

    GLuint shader = load_shader(VERTEX_GLSL, FRAGMENT_GLSL);

    vec3f_t camera = {{ 10.0f, 10.0f, 10.0f }};
    vec3f_t focus = {{ 0.0f, 0.0f, 0.0f }};
    vec3f_t up = {{ 0.0f, 1.0f, 0.0f }};
    vec3f_t light = {{ 10.0f, 10.0f, 10.0f }};

    glUniform4fv(glGetUniformLocation(shader, "u_CameraPos"), 1, (float *)camera.data);
    glUniform4fv(glGetUniformLocation(shader, "u_LightPos"), 1, (float *)light.data);

    model_t model;
    model_init(&model);
    
    if (argc > 1) {
        model_load(&model, argv[1]);
    }
    
    mat4x4_t view;
    mat4x4_t projection;

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

        vec3f_t axis = {{ 0.0f, 1.0f, 0.0f }};
        mat4x4_rotate(&(model.transform), 0.001f, &axis);

        model_render(&model, shader, &view, &projection);

        SDL_GL_SwapWindow(window);
    }

    model_term(&model);

    return 0;
}