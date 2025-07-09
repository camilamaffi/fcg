#include "player.h"
#include <cstdio>

player::player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor, SceneObject objeto) : entity(atributos_jogador.health_points, atributos_jogador.attack_power, atributos_jogador.damage_resistance, atributos_jogador.speed, starter_position, starter_view_direction, starter_scaling_factor, objeto)
{
    setPlayerPosition(starter_position);
    setHasDragonPower(false);
    this->momento_da_ultima_chamada_valida = 0.0f;
}

player::~player()
{
    //dtor
}

bool player::attackIsValid(double momento_atual){
    if(momento_atual - momento_da_ultima_chamada_valida > tempo_para_jogador_atacar_de_novo){
        momento_da_ultima_chamada_valida = momento_atual;
        return true;
    }
    return false;
}
