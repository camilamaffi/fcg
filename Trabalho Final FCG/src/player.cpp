#include "player.h"

player::player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction) : entity(atributos_jogador.health_points, atributos_jogador.attack_power, atributos_jogador.damage_resistance, atributos_jogador.speed, starter_position, starter_view_direction)
{
    setPlayerPosition(starter_position);
}

player::~player()
{
    //dtor
}
