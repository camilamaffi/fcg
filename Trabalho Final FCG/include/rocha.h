#ifndef ROCHA_H
#define ROCHA_H

#include "object.h"

class rocha : public object{
    public:
        rocha(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto);
        virtual ~rocha();

        glm::vec4 getTransformedBboxMin() { return this->getModel() * glm::vec4(this->getBoundingBox().getBboxMin(),1.0f); }
        glm::vec4 getTransformedBboxMax() { return this->getModel() * glm::vec4(this->getBoundingBox().getBboxMax(),1.0f); }
    private:
};

#endif // ROCHA_H
