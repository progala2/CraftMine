/*
 * OpenGl.h
 *
 *  Created on: Oct 8, 2014
 *      Author: p.rogala
 */

#ifndef XKS_OPENGL_H_
#define XKS_OPENGL_H_

#include "stdafx.h"
#include "Application.h"

namespace XKS {

class OpenGL {
 public:
     static std::shared_ptr<OpenGL> GetInstance() {
         if (ms_instance == nullptr)
             ms_instance.reset(new OpenGL());
         return ms_instance;
     }

     ~OpenGL() {
         ms_instance = nullptr;
     }

     void UpdateProjectionMatrix(const float& FoV, const float& aspect, const float& zNear, const float& clippingDistance) {
         m_projectionMatrix = glm::perspective(FoV, aspect, zNear, clippingDistance);
     }

     const glm::mat4& GetProjectionMatrix() const {
        return m_projectionMatrix;
     }
 private:

     OpenGL();

     glm::mat4 m_projectionMatrix;

     static std::shared_ptr<OpenGL> ms_instance;
};

}



#endif /* OPENGL_H_ */
