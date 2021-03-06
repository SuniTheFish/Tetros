#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <glad/glad.h>

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif // !DEBUG_H
