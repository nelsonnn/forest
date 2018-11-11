#ifndef SHADER_H
#define SHADER_H

#include "stdGL.h"

// this function stolen from 4229 class examples
char* ReadText(char* file);
// this function stolen from 4229 class examples
int CreateShader(GLenum type, char* file);
// this function stolen (mostly) from 4229 class examples
int CreateShaderProg(char* VertFile, char* FragFile);

#endif
