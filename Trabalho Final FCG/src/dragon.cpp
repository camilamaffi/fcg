#include "dragon.h"

const float dragon_detection_radius = 50.0f;

dragon::dragon(STANDARD_DRAGON_ATTRIBUTES atributos_dragao, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor, SceneObject objeto) : enemy(atributos_dragao.health_points, atributos_dragao.attack_power, atributos_dragao.damage_resistance, atributos_dragao.speed, dragon_detection_radius, starter_position, starter_view_direction, starter_scaling_factor, objeto)
{
    this->momento_do_ultimo_ataque = 0.0;
    this->can_die_to_fireball = true;
}

dragon::~dragon()
{

}

bool dragon::attackIsValid(double momento_atual){
    if(momento_atual - momento_do_ultimo_ataque > intervalo_entre_ataques){
        momento_do_ultimo_ataque = momento_atual;
        return true;
    }
    return false;
}
