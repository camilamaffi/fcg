#ifndef KRAKEN_H
#define KRAKEN_H

#include <enemy.h>


class kraken : public enemy
{
    public:
        /** Default constructor */
        kraken(int health_points, int attack_power, int damage_resistance, float speed, float kraken_detection_radius, glm::vec4 starter_position, glm::vec4 starter_view_direction);
        /** Default destructor */
        virtual ~kraken();

    protected:

    private:
};

#endif // KRAKEN_H
