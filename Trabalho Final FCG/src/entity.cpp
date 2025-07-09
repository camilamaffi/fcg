#include "entity.h"

entity::entity(int health_points, int attack_power, int damage_resistance, float speed, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor, SceneObject objeto) : object(speed, starter_position, starter_scaling_factor, starter_view_direction, objeto)
{
    setHealthPoints(health_points);
    setAttackPower(attack_power);
    setDamageResistance(damage_resistance);
}

entity::~entity()
{
    //dtor
}

