#ifndef SQUID_H
#define SQUID_H

#include <enemy.h>

struct atributos_base_lula{
    const int health_points = 5;
    const int attack_power = 3;
    const int damage_resistance = 0;
    const float speed = 1.0f;
}typedef STANDARD_SQUID_ATTRIBUTES;

class squid : public enemy
{
    public:
        squid(STANDARD_SQUID_ATTRIBUTES atributos_lula, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor, SceneObject objeto);
        virtual ~squid();

        glm::vec4 getFirstSquidPosition() {return this->first_squid_position ;}
        glm::vec4 getFirstSquidViewDirection() {return this->first_squid_view_direction ;}

        glm::vec4 getTransformedBsphereCentro() { return this->getModel() * glm::vec4(this->getBoundingSphere().getCentro()); }
        float getTransformedBsphereRaio() { return this->getScalingFactor().x * this->getBoundingSphere().getRaio(); }

    protected:

    private:
        glm::vec4 first_squid_position;
        glm::vec4 first_squid_view_direction;
};

#endif // SQUID_H
