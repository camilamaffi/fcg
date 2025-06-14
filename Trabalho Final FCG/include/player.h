#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct atributos_base_jogador{
    const int health_points = 10;
    const int attack_power = 5;
    const int damage_resistance = 1;
    const float speed = 0.1f;
}typedef STANDARD_PLAYER_ATTRIBUTES;

class player : public entity
{
    public:
        /** Default constructor */
        player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction);
        /** Default destructor */
        virtual ~player();

        void setPlayerPosition(glm::vec4 new_position) { this->player_position = new_position ;}
        void setPlayerViewDirection(glm::vec4 new_view_direction) {this->player_view_direction = new_view_direction ;}
        glm::vec4 getPlayerPosition() {return this->player_position ;}
        glm::vec4 getPlayerViewDirection() {return this->player_view_direction ;}

    protected:

    private:
        glm::vec4 player_position;
        glm::vec4 player_view_direction;
};

#endif // PLAYER_H
