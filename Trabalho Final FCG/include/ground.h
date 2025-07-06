#ifndef GROUND_H
#define GROUND_H

#include "object.h"

class ground : public object{
    public:
        ground(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        ~ground();
    private:
};

#endif // GROUND_H
