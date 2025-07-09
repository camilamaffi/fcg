#ifndef GROUND_H
#define GROUND_H

#include "object.h"

class ground : public object{
    public:
        ground(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto);
        ~ground();

        glm::vec4 getTransformedBplaneMin() { return this->getModel() * glm::vec4(this->getBoundingPlane().getBplaneMin(),1.0f); }
        glm::vec4 getTransformedBplaneMax() { return this->getModel() * glm::vec4(this->getBoundingPlane().getBplaneMax(),1.0f); }
    private:
};

#endif // GROUND_H
