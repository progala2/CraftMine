#ifndef _XKS_CAMERA_H_
#define _XKS_CAMERA_H_

#include "stdafx.h"

	struct Camera
	{
		// position
		glm::vec3 m_pos, m_dir;
		// horizontal angle
		float m_hAngle;
		// vertical angle
		float m_vAngle;
		// Initial Field of View
		float m_FoV;		

		Camera(glm::vec3 pos, float FoV): m_pos(pos), m_hAngle(-1.f), m_vAngle(-1.f),
			m_FoV(FoV) { m_dir = GetDirection();}

		glm::vec3 GetDirection() const { return glm::vec3(cos(m_vAngle) * sin(m_hAngle),
															sin(m_vAngle),
															cos(m_vAngle) * cos(m_hAngle)
															); }
		glm::vec3 GetRightVec() const { return glm::vec3(sin(m_hAngle - 3.14f*0.5f),
															0,
															cos(m_hAngle - 3.14f*0.5f)
															); }
		glm::vec3 GetUpVec() const {return glm::cross( GetRightVec(), GetDirection() ); }

		glm::mat4 LookAt() const {return glm::lookAt(m_pos, m_pos + m_dir, GetUpVec()); }
	};

#endif