#ifndef _XKS_T2_SHADER_H_
#define _XKS_T2_SHADER_H_

#include "ShaderProgram.h"
#include "stdafx.h"

	class CubeShader : public ShaderProgram
	{
	public:
		CubeShader();
		void SetVision(const glm::mat4& V);
		void SetProjection(const glm::mat4& P);
		void SetWorldPos(const glm::vec3& pos);
		void SetNormal(const glm::vec3& pos);
		void SetLight(const glm::vec3& pos);
		void SetTexture() {}

	private:
		GLint m_viewMatrixLoc;
		GLint m_projectionMatrixLoc;
		GLint m_worldVectorLoc;
		GLint m_normalVectorLoc;
		GLint m_lightVectorLoc;
	};

#endif