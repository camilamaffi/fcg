#include "enemy.h"

enemy::enemy(int health_points, int attack_power, int damage_resistance, float speed, float detection_radius, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor, SceneObject objeto) : entity(health_points, attack_power, damage_resistance, speed, starter_position, starter_view_direction, starter_scaling_factor, objeto)
{
    this->detection_radius = detection_radius;
    this->is_alive = true;;
}

enemy::~enemy()
{
    //dtor
}
