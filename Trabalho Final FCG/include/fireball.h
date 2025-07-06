#ifndef FIREBALL_H
#define FIREBALL_H

#include "object.h"

const float velocidade_bola_de_fogo_dinamica = 5.0f;

class fireball : public object{
    public:
        fireball(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        ~fireball();
    private:
};

#endif // FIREBALL_H
