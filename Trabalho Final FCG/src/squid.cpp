#include "squid.h"

squid::squid(int health_points, int attack_power, int damage_resistance, float speed) : enemy(health_points, attack_power, damage_resistance, speed)
{
    //ctor
}

squid::~squid()
{
    //dtor
}
