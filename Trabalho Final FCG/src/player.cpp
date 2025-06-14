#include "player.h"

player::player(STANDARD_PLAYER_ATTRIBUTES atributos_jogador, glm::vec4 starter_position, glm::vec4 starter_view_direction) : entity(atributos_jogador.health_points, atributos_jogador.attack_power, atributos_jogador.damage_resistance, atributos_jogador.speed)
{
    setPlayerPosition(starter_position);
    setPlayerViewDirection(starter_view_direction);
}

player::~player()
{
    //dtor
}
