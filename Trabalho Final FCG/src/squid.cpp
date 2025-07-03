#include "squid.h"

const float squid_detection_radius = 10.0f;

squid::squid(STANDARD_SQUID_ATTRIBUTES atributos_lula, glm::vec4 starter_position, glm::vec4 starter_view_direction) : enemy(atributos_lula.health_points, atributos_lula.attack_power, atributos_lula.damage_resistance, atributos_lula.speed, squid_detection_radius, starter_position, starter_view_direction)
{
    setPosition(starter_position);
    setViewDirection(starter_view_direction);
    this->first_squid_position = starter_position;
    this->first_squid_view_direction = starter_view_direction;
}

squid::~squid()
{
    //dtor
}

