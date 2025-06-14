#include "entity.h"

entity::entity(int health_points, int attack_power, int damage_resistance, float speed)
{
    this->health_points = health_points;
    this->attack_power = attack_power;
    this->damage_resistance = damage_resistance;
    this-> speed = speed;
}

entity::~entity()
{
    //dtor
}
