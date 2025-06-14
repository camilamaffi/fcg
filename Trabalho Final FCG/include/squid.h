#ifndef SQUID_H
#define SQUID_H

#include <enemy.h>


class squid : public enemy
{
    public:
        /** Default constructor */
        squid(int health_points, int attack_power, int damage_resistance, float speed);
        /** Default destructor */
        virtual ~squid();

    protected:

    private:
};

#endif // SQUID_H
