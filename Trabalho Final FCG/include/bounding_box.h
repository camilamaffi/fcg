#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class bounding_box{
    public:
        bounding_box(glm::vec3 bbox_min, glm::vec3 bbox_max);

        glm::vec3 getBboxMin() { return this->bbox_min; }
        glm::vec3 getBboxMax() { return this->bbox_max; }

    private:
        glm::vec3 bbox_min;
        glm::vec3 bbox_max;
};

#endif // BOUNDING_BOX_H
