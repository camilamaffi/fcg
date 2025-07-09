#ifndef BOUNDING_PLANE_H
#define BOUNDING_PLANE_H

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class bounding_plane{
    public:
        bounding_plane(glm::vec3 bbox_min, glm::vec3 bbox_max);

        glm::vec3 getBplaneMin() { return this->bplane_min; }
        glm::vec3 getBplaneMax() { return this->bplane_max; }

    private:
        glm::vec3 bplane_min;
        glm::vec3 bplane_max;
};

#endif // BOUNDING_PLANE_H

