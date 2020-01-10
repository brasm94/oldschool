#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include "glitter.hpp"

// Loads GLSL vertex and fragment shaders from the provided files and checks for errors. 
// Returns the new program ID on success or 0 on failure.
GLuint LoadProgram(const char* vertexShaderFile, const char* fragmentShaderFile);

inline void UseProgram(GLuint shaderID)
{
	glUseProgram(shaderID);
}

inline void DeleteProgram(GLuint shaderID)
{
	glDeleteProgram(shaderID);
}

#endif
