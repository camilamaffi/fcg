#include "entity.h"

entity::entity(int health_points, int attack_power, int damage_resistance, float speed, glm::vec4 starter_position, glm::vec4 starter_view_direction)
{
    setHealthPoints(health_points);
    setAttackPower(attack_power);
    setDamageResistance(damage_resistance);
    setSpeed(speed);
    setPosition(starter_position);
    setViewDirection(starter_view_direction);
}

entity::~entity()
{
    //dtor
}

