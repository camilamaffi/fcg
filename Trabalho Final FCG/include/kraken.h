#ifndef KRAKEN_H
#define KRAKEN_H

#include <enemy.h>


class kraken : public enemy
{
    public:
        /** Default constructor */
        kraken(int health_points, int attack_power, int damage_resistance, float speed);
        /** Default destructor */
        virtual ~kraken();

    protected:

    private:
};

#endif // KRAKEN_H
