#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>


class enemy : public entity
{
    public:
        /** Default constructor */
        enemy(int health_points, int attack_power, int damage_resistance, float speed);
        /** Default destructor */
        virtual ~enemy();

    protected:

    private:
};

#endif // ENEMY_H
