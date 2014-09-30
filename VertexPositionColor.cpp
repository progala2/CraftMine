#include "stdafx.h"
#include "VertexPositionColor.h"

namespace XKS {

const VertexAttribDescription VertexPositionColor::Attributes[VertexPositionColor::AttributesCount] = {
        VertexAttribDescription(3, GL_FLOAT, (const GLvoid*) offsetof(VertexPositionColor, m_position)),
        VertexAttribDescription(4, GL_FLOAT, (const GLvoid*) offsetof(VertexPositionColor, m_color)) };

const VertexAttribDescription VertexPositionTexture::Attributes[VertexPositionTexture::AttributesCount] = {
        VertexAttribDescription(3, GL_FLOAT, (const GLvoid*) offsetof(VertexPositionTexture, m_position)),
        VertexAttribDescription(2, GL_FLOAT, (const GLvoid*) offsetof(VertexPositionTexture, m_texture)) };

const VertexAttribDescription VertPosTexCompressed::Attributes[VertPosTexCompressed::AttributesCount] = {
        VertexAttribDescription(3, GL_UNSIGNED_BYTE, (const GLvoid*) offsetof(VertPosTexCompressed, m_position)),
        VertexAttribDescription(2, GL_UNSIGNED_SHORT, (const GLvoid*) offsetof(VertPosTexCompressed, m_texture))

};

}
