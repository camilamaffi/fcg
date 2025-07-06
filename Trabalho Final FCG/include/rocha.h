#ifndef ROCHA_H
#define ROCHA_H

#include "object.h"

class rocha : public object{
    public:
        rocha(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        virtual ~rocha();
    private:
};

#endif // ROCHA_H
