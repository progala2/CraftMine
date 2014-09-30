#ifndef _XKS_SHADER_PROGRAM_H_
#define _XKS_SHADER_PROGRAM_H_

#include "stdafx.h"

namespace XKS {

class ShaderProgram {
 public:
    ShaderProgram(const char *vsFile, const char *fsFile);
    void Attach();
    void Destroy();
    //Dodana metoda
    GLint GetUniform(const char *uniformName);
    GLint GetAttrib(const char *attribName);
    //Dodany kwalifikator virtual
    virtual ~ShaderProgram();

 private:
    std::string FileContent(const char *filename);
    void PrintInfoLog(GLuint shaderID);
    GLuint CompileShader(GLenum shaderType, const char* filename);

    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
    GLuint m_programID;
    std::mutex m_mtx;
};

}
#endif
