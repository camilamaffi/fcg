#ifndef BEZIER_BUILDER_H
#define BEZIER_BUILDER_H

#include <glm/vec4.hpp>

class bezierBuilder{
    public:
        bezierBuilder();
        virtual ~bezierBuilder();

        glm::vec4 cubicBezier(const glm::vec4& p0, const glm::vec4& p1, const glm::vec4& p2, const glm::vec4& p3, float t);

    private:

};

#endif // BEZIER_BUILDER_H
