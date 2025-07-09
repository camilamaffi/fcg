#ifndef PEDESTAL_H
#define PEDESTAL_H

#include "object.h"

class pedestal : public object{
    public:
        pedestal(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto);
        virtual ~pedestal();

        glm::vec4 getTransformedBboxMin() { return this->getModel() * glm::vec4(this->getBoundingBox().getBboxMin(),1.0f); }
        glm::vec4 getTransformedBboxMax() { return this->getModel() * glm::vec4(this->getBoundingBox().getBboxMax(),1.0f); }
    private:
};
#endif // PEDESTAL_H
