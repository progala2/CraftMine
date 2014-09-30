#ifndef _XKS_UTILITY_H_
#define _XKS_UTILITY_H_
#include "stdafx.h"

struct HashVector;

typedef GLushort BlockTypeNumber;
typedef glm::detail::tvec3<GLubyte, glm::precision::defaultp> Vec3ub;
typedef glm::detail::tvec2<BlockTypeNumber, glm::precision::defaultp> Vec2us;

struct Vec2ub {
    Vec2ub(GLubyte _x, GLubyte _y, GLubyte _z)
            : xz(_x + (_z << 4)),
              y(_y) {
    }
    explicit Vec2ub(GLubyte _xz = 0, GLubyte _y = 0)
            : xz(_xz),
              y(_y) {
    }
    GLubyte xz, y;

};

bool operator==(const Vec2ub& a, const Vec2ub& b);

bool operator<(const Vec2ub& a, const Vec2ub& b);

typedef google::dense_hash_map<Vec2ub, BlockTypeNumber, HashVector, std::equal_to<Vec2ub>,
        std::allocator<std::pair<const Vec2ub, BlockTypeNumber> > > ChunkData;

struct HashVector {
    HashVector() {
    }
    std::size_t operator()(Vec2ub const& a) const {
        int b = a.xz + (int(a.y) << 8);
        return b % 982451653;
    }
    bool operator( )(const Vec2ub& _Key1, const Vec2ub& _Key2) const {
        return (_Key1 < _Key2);
    }
};

namespace glm {
namespace detail {
template<class T, precision P>
bool operator<(const tvec3<T, P>& a, const tvec3<T, P>& b) {
    if (a.x < b.x)
        return true;
    if (a.x > b.x)
        return false;
    if (a.z < b.z)
        return true;
    if (a.z > b.z)
        return false;
    if (a.y < b.y)
        return true;
    return false;
}
}  // detail
    template<class T>
    const T& ZERO() {
        static T zero(0);

        return zero;
    }
}  // glm
;
#endif
