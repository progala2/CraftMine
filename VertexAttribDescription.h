#ifndef _XKS_VERTEX_ATTRIB_DESCRIPTION_H_
#define _XKS_VERTEX_ATTRIB_DESCRIPTION_H_

#include "stdafx.h"

namespace XKS {

struct VertexAttribDescription {
 public:
    VertexAttribDescription(GLint _size, GLenum _type, const GLvoid *_offset)
            : size(_size),
              type(_type),
              normalized(GL_FALSE),
              offset(_offset) {
    }

    VertexAttribDescription(GLint _size, GLenum _type, GLboolean _normalized, const GLvoid *_offset)
            : size(_size),
              type(_type),
              normalized(_normalized),
              offset(_offset) {
    }

    GLint size;  //number of components
    GLenum type;  //type of the components
    GLboolean normalized;  //normalize fixed-point data
    const GLvoid *offset;  //offset of the attribute in the vertex structure
};

}
#endif
