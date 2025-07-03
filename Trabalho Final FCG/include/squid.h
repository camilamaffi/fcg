#ifndef SQUID_H
#define SQUID_H

#include <enemy.h>

struct atributos_base_lula{
    const int health_points = 5;
    const int attack_power = 3;
    const int damage_resistance = 0;
    const float speed = 1.0f;
}typedef STANDARD_SQUID_ATTRIBUTES;

class squid : public enemy
{
    public:
        /** Default constructor */
        squid(STANDARD_SQUID_ATTRIBUTES atributos_lula, glm::vec4 starter_position, glm::vec4 starter_view_direction);
        /** Default destructor */
        virtual ~squid();

        glm::vec4 getFirstSquidPosition() {return this->first_squid_position ;}
        glm::vec4 getFirstSquidViewDirection() {return this->first_squid_view_direction ;}

    protected:

    private:
        glm::vec4 first_squid_position;
        glm::vec4 first_squid_view_direction;
};

#endif // SQUID_H
