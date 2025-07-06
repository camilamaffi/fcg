#include "kraken.h"

const float kraken_detection_radius = 0.0f;

kraken::kraken(STANDARD_KRAKEN_ATTRIBUTES atributos_kraken, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor) : enemy(atributos_kraken.health_points, atributos_kraken.attack_power, atributos_kraken.damage_resistance, atributos_kraken.speed, kraken_detection_radius, starter_position, starter_view_direction, starter_scaling_factor)
{
    //ctor
}

kraken::~kraken()
{
    //dtor
}
