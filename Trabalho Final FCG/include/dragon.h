#ifndef DRAGON_H
#define DRAGON_H

#include <enemy.h>

struct atributos_base_dragao{
    const int health_points = 5;
    const int attack_power = 3;
    const int damage_resistance = 0;
    const float speed = 1.0f;
}typedef STANDARD_DRAGON_ATTRIBUTES;

class dragon : public enemy
{
    public:
        dragon(STANDARD_DRAGON_ATTRIBUTES atributos_dragao, glm::vec4 starter_position, glm::vec4 starter_view_direction, glm::vec3 starter_scaling_factor);
        virtual ~dragon();

        double getIntervaloEntreAtaques() { return this->intervalo_entre_ataques; }

        bool attackIsValid(double momento_atual);

    private:
        const double intervalo_entre_ataques = 3.0;
        double momento_do_ultimo_ataque;
};

#endif // DRAGON_H
