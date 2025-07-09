#ifndef FIREBALL_H
#define FIREBALL_H

#include "object.h"

const float velocidade_bola_de_fogo_dinamica = 5.0f;

class fireball : public object{
    public:
        fireball(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto);
        ~fireball();

        glm::vec4 getTransformedBsphereCentro() { return this->getModel() * glm::vec4(this->getBoundingSphere().getCentro()); }
                            // são iguais para todos os lados, então só multiplicamos o raio pelo valor de algum dos escalamentos
        float getTransformedBsphereRaio() { return this->getScalingFactor().x * this->getBoundingSphere().getRaio(); }
    private:
};

#endif // FIREBALL_H
