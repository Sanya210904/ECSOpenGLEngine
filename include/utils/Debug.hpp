#pragma once

#include <glad/glad.h>
#include <string>

#define checkError() checkError_(__FILE__, __LINE__)

std::string GLErrorToString(GLenum error);
std::string GLFramebufferStatus(GLenum error);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei lenght, const char* message, const void* userParam);

GLenum checkError_(const char* file, int line);

#define ENABLE_DEBUG                                                                               \
    int openglFlags;                                                                               \
    glGetIntegerv(GL_CONTEXT_FLAGS, &openglFlags);                                                 \
    if (openglFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {                                                 \
        glEnable(GL_DEBUG_OUTPUT);                                                                 \
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);                                                     \
        glDebugMessageCallback(glDebugOutput, nullptr);                                            \
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);      \
    }
