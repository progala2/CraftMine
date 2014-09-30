#include "stdafx.h"
#include "Utility.h"

bool operator==(const Vec2ub& a, const Vec2ub& b) {
    return !(b.xz != a.xz || b.y != a.y);
}

bool operator<(const Vec2ub& a, const Vec2ub& b) {
    if (a.xz < b.xz)
        return true;
    if (a.xz > b.xz)
        return false;
    if (a.y < b.y)
        return true;
    return false;
}
