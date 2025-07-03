#include "kraken.h"

kraken::kraken(int health_points, int attack_power, int damage_resistance, float speed, float kraken_detection_radius, glm::vec4 starter_position, glm::vec4 starter_view_direction) : enemy(health_points, attack_power, damage_resistance, speed, kraken_detection_radius, starter_position, starter_view_direction)
{
    //ctor
}

kraken::~kraken()
{
    //dtor
}
