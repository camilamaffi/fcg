#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct atributos_base_jogador{
    const int health_points = 10;
    const int attack_power = 5;
    const int damage_resistance = 1;
    const double speed = 5.0;
}typedef STANDARD_PLAYER_ATTRIBUTES;

class player : public entity
{
    public:
        /** Default constructor */
        player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction);
        /** Default destructor */
        virtual ~player();

        void setPlayerPosition(glm::vec4 new_position) {
            setPosition(new_position);
            player_head_position.x = new_position.x;
            player_head_position.y = new_position.y+5.0f;
            player_head_position.z = new_position.z;
            player_head_position.w = 1.0f;
        }
        glm::vec4 getPlayerHeadPosition() {return this->player_head_position ;}

        /** Access w_vector
         * \return The current value of w_vector
         */
        glm::vec4 getPlayerWVector() { return w_vector; }

        /** Set w_vector
         * \param w_vector New value to set
         */
        void setPlayerWVector(glm::vec4 w_vector) { this->w_vector = w_vector; }

        /** Access u_vector
         * \return The current value of u_vector
         */
        glm::vec4 getPlayerUVector() { return u_vector; }

        /** Set u_vector
         * \param u_vector New value to set
         */
        void setPlayerUVector(glm::vec4 u_vector) { this->u_vector = u_vector; }

    protected:

    private:
        glm::vec4 player_head_position;
        glm::vec4 w_vector; //!< Member variable "w_vector"
        glm::vec4 u_vector; //!< Member variable "u_vector"
};

#endif // PLAYER_H
