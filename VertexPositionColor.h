#ifndef _XKS_VERTEX_POSITION_COLOR_H_
#define _XKS_VERTEX_POSITION_COLOR_H_

#include "Utility.h"
#include "stdafx.h"
#include "VertexAttribDescription.h"

namespace XKS {

struct VertexPositionColor {
 public:
    VertexPositionColor(const glm::vec3& position, const glm::vec4& color)
            : m_position(position),
              m_color(color) {
    }

    glm::vec3 m_position;
    glm::vec4 m_color;

    //Dodane dwa pola statyczne
    static const int AttributesCount = 2;
    static const VertexAttribDescription Attributes[AttributesCount];
};

struct VertexPositionTexture {
 public:
    VertexPositionTexture(const glm::vec3& position, const glm::vec2& tex)
            : m_position(position),
              m_texture(tex) {
    }

    glm::vec3 m_position;
    glm::vec2 m_texture;

    //Dodane dwa pola statyczne
    static const int AttributesCount = 2;
    static const VertexAttribDescription Attributes[AttributesCount];
};

struct VertPosTexCompressed {
 public:
    VertPosTexCompressed(const Vec3ub& position, const Vec2us& tex)
            : m_position(position),
              m_texture(tex) {
    }

    Vec3ub m_position;
    Vec2us m_texture;

    //Dodane dwa pola statyczne
    static const int AttributesCount = 2;
    static const VertexAttribDescription Attributes[AttributesCount];
};

}
#endif
