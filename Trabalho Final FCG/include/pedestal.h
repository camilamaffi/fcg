#ifndef PEDESTAL_H
#define PEDESTAL_H

#include "object.h"

class pedestal : public object{
    public:
        pedestal(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        virtual ~pedestal();
    private:
};
#endif // PEDESTAL_H
