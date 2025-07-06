#ifndef KRAKEN_H
#define KRAKEN_H

#include <enemy.h>

struct atributos_base_kraken{
    const int health_points = 999;
    const int attack_power = 999;
    const int damage_resistance = 999;
    const float speed = 0.0f;
}typedef STANDARD_KRAKEN_ATTRIBUTES;


class kraken : public enemy
{
    public:
        kraken(STANDARD_KRAKEN_ATTRIBUTES atributos_kraken, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor);
        virtual ~kraken();

    protected:

    private:
};

#endif // KRAKEN_H
