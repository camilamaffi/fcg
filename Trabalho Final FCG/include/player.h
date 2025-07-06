#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct atributos_base_jogador{
    const int health_points = 10;
    const int attack_power = 5;
    const int damage_resistance = 1;
    const double speed = 50.0;
}typedef STANDARD_PLAYER_ATTRIBUTES;

class player : public entity
{
    public:
        player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor);
        virtual ~player();

        void setPlayerPosition(glm::vec4 new_position) {
            setPosition(new_position);

            player_head_position.x = new_position.x;
            player_head_position.y = new_position.y+5.0f;
            player_head_position.z = new_position.z;
            player_head_position.w = 1.0f;

            player_chest_position.x = new_position.x;
            player_chest_position.y = new_position.y+3.5f;
            player_chest_position.z = new_position.z;
            player_chest_position.w = 1.0f;
        }
        glm::vec4 getPlayerHeadPosition() {return this->player_head_position ;}
        glm::vec4 getPlayerChestPosition() {return this->player_chest_position ;}

        glm::vec4 getPlayerWVector() { return w_vector; }
        void setPlayerWVector(glm::vec4 w_vector) { this->w_vector = w_vector; }

        glm::vec4 getPlayerUVector() { return u_vector; }
        void setPlayerUVector(glm::vec4 u_vector) { this->u_vector = u_vector; }

        bool getHasDragonPower() { return this->has_dragon_power ;}
        void setHasDragonPower( bool has_dragon_power ) { this->has_dragon_power = has_dragon_power; }

        bool attackIsValid(double momento_atual);

        double getTempoParaJogadorAtacarDeNovo() { return this->tempo_para_jogador_atacar_de_novo; }


    protected:

    private:
        glm::vec4 player_head_position;
        glm::vec4 player_chest_position;
        glm::vec4 w_vector;
        glm::vec4 u_vector;
        double momento_da_ultima_chamada_valida;
        bool has_dragon_power;
        const double tempo_para_jogador_atacar_de_novo = 0.5;
};

#endif // PLAYER_H
