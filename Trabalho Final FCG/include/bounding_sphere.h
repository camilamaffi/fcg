#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"

class bounding_sphere{
    public:
        bounding_sphere(glm::vec3 bbox_min, glm::vec3 bbox_max);
        glm::vec4 getCentro() { return this->centro; }
        float getRaio() { return this->raio; }

    private:
        glm::vec4 centro;
        float raio;
};

#endif // BOUNDING_SPHERE_H
