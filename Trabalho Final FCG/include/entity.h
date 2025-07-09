#ifndef ENTITY_H
#define ENTITY_H

#include "object.h"

class entity : public object
{
    public:
        entity(int health_points, int attack_power, int damage_resistance, float speed, glm::vec4 starter_position, glm::vec4 view_direction, glm::vec3 scaling_factor, SceneObject objeto);
        virtual ~entity();

        int getHealthPoints() { return health_points; }
        void setHealthPoints(int health_points) { this->health_points = health_points; }

        int getAttackPower() { return attack_power; }
        void setAttackPower(int attack_power) { this->attack_power = attack_power; }

        int getDamageResistance() { return damage_resistance; }
        void setDamageResistance(int damage_resistance) { this->damage_resistance = damage_resistance; }

    protected:

    private:
        int health_points;
        int attack_power;
        int damage_resistance;
};

#endif // ENTITY_H
