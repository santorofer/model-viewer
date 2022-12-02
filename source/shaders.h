#ifndef SHADERS_H
#define SHADERS_H

// OpenGL Shading Language (GLSL)
// https://en.wikipedia.org/wiki/OpenGL_Shading_Language

const char * VERTEX_SHADER = 
"#version 410 core                                              \n"
"layout(location = 0) uniform vec4 u_MVP;                       \n"
"                                                               \n"
"layout(location = 0) in vec3 a_Position;                       \n"
"layout(location = 1) in vec3 a_Normal;                         \n"
"layout(location = 2) in vec2 a_TexCoord;                       \n"
"                                                               \n"
"layout(location = 0) out vec3 v_Position;                      \n"
"layout(location = 2) out vec3 v_Normal;                        \n"
"layout(location = 1) out vec2 v_TexCoord;                      \n"
"                                                               \n"
"void main() {                                                  \n"
"  gl_Position = vec4(a_Position, 1.0) * u_MVP;                 \n"
"  v_Position = a_Position;                                     \n"
"  v_Normal = a_Normal;                                         \n"
"  v_TexCoord = a_TexCoord;                                     \n"
"}                                                              \n"
;

const char * FRAGMENT_SHADER = 
"#version 410 core                                              \n"
"layout(location = 1) uniform sampler2D u_DiffuseMap;           \n"
"                                                               \n"
"layout(location = 0) out vec3 v_Position;                      \n"
"layout(location = 2) out vec3 v_Normal;                        \n"
"layout(location = 1) out vec2 v_TexCoord;                      \n"
"                                                               \n"
"layout(location = 0) out vec4 o_Color;                         \n"
"                                                               \n"
"void main() {                                                  \n"
"  o_Color = texture(u_DiffuseMap, v_TexCoord);                 \n"
"  vec3 lightDir = normalize(u_LightPos - v_Position);          \n"
"  vec3 viewDir = normalize(u_CameraPos - v_Position);          \n"
"  vec3 halfwayDir = normalize(lightDire + viewDirection);      \n"
"}                                                              \n"
;

#endif // SHADERS_H