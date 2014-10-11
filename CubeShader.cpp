#include "stdafx.h"
#include "CubeShader.h"

using namespace glm;

namespace XKS {

CubeShader::CubeShader()
        : ShaderProgram("Cube_vs.glsl", "Cube_fs.glsl") {
    m_viewMatrixLoc = GetUniform("V");
    m_projectionMatrixLoc = GetUniform("P");
    m_worldVectorLoc = GetUniform("worldpos");

    m_normalVectorLoc = GetUniform("myNormal");
    m_lightVectorLoc = GetUniform("LightPosition_worldspace");
}

void CubeShader::SetVision(const glm::mat4& V) {
    glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(V));
}

void CubeShader::SetProjection(const glm::mat4& P) {
    glUniformMatrix4fv(m_projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(P));
}

void CubeShader::SetWorldPos(const glm::vec3& pos) {
    glUniform3fv(m_worldVectorLoc, 1, glm::value_ptr(pos));
}

void CubeShader::SetNormal(const glm::vec3& pos) {
    glUniform3fv(m_normalVectorLoc, 1, glm::value_ptr(pos));
}

void CubeShader::SetLight(const glm::vec3& pos) {
    glUniform3fv(m_lightVectorLoc, 1, glm::value_ptr(pos));
}

}