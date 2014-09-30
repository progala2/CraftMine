#include "stdafx.h"
#include "ShaderProgram.h"

using namespace std;

namespace XKS {

void ShaderProgram::PrintInfoLog(GLuint shaderID) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shaderID, infoLogLen, &charsWritten, infoLog);
        cout << "InfoLog : " << endl << infoLog << endl;
        delete[] infoLog;
    }
}

string ShaderProgram::FileContent(const char *filename) {
    ifstream in;
    in.exceptions(ios::badbit | ios::failbit);
    in.open(filename, ios::in | ios::binary);
    string content;
    in.seekg(0, std::ios::end);
    content.resize(unsigned(in.tellg()));
    in.seekg(0, std::ios::beg);
    in.read(&content[0], content.size());
    in.close();
    return content;
}

GLuint ShaderProgram::CompileShader(GLenum shaderType, const char* filename) {
    GLuint shaderID = glCreateShader(shaderType);
    if (shaderID == 0)
        throw exception("Blad glCreateShader!");
    string filecontent = FileContent(filename);
    const char *fc = filecontent.c_str();
    glShaderSource(shaderID, 1, &fc, NULL);
    glCompileShader(shaderID);
    GLint compiled;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        PrintInfoLog(shaderID);
        throw exception("Blad kompilacji shadera!");
    }
    return shaderID;
}

ShaderProgram::ShaderProgram(const char *vsFile, const char *fsFile)
        : m_vertexShaderID(0),
          m_fragmentShaderID(0),
          m_programID(0) {
    m_vertexShaderID = CompileShader(GL_VERTEX_SHADER, vsFile);
    m_fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fsFile);
    m_programID = glCreateProgram();
    if (m_programID == 0)
        throw exception("Blad glCreateProgram!");
    GLenum err = glGetError();
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);
    glLinkProgram(m_programID);
    err = glGetError();
    if (err != GL_NO_ERROR)
        throw exception("Blad programu shaderow!");
}

void ShaderProgram::Attach() {
    GLenum err = glGetError();
    glUseProgram(m_programID);
    err = glGetError();
    if (err != GL_NO_ERROR)
        throw exception("Blad glUseProgram!");
}

void ShaderProgram::Destroy() {
    if (m_programID) {
        GLuint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&currentProgram));
        if (currentProgram == m_programID)
            glUseProgram(0);
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
    if (m_fragmentShaderID) {
        glDeleteShader(m_fragmentShaderID);
        m_fragmentShaderID = 0;
    }
    if (m_vertexShaderID) {
        glDeleteShader(m_vertexShaderID);
        m_vertexShaderID = 0;
    }
}

ShaderProgram::~ShaderProgram() {
    Destroy();
}

GLint ShaderProgram::GetUniform(const char *uniformName) {
    m_mtx.lock();
    GLenum err = glGetError();
    GLint loc = glGetUniformLocation(m_programID, uniformName);
    err = glGetError();
    if (loc < 0 || err != GL_NO_ERROR)
        throw exception("Blad glGetUniformLocation!");
    m_mtx.unlock();
    return loc;
}

GLint ShaderProgram::GetAttrib(const char *attribName) {
    m_mtx.lock();
    GLenum err = glGetError();
    GLint loc = glGetAttribLocation(m_programID, attribName);
    err = glGetError();
    if (loc < 0 || err != GL_NO_ERROR)
        throw exception("Blad glGetAttribLocation!");
    m_mtx.unlock();
    return loc;
}

}
