#ifndef CAVE_H
#define CAVE_H

#include "object.h"

class cave : public object{
    public:
        cave(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        ~cave();
    private:
};

#endif // CAVE_H
