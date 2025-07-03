#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>


class enemy : public entity
{
    public:
        /** Default constructor */
        enemy(int health_points, int attack_power, int damage_resistance, float speed, float detection_radius, glm::vec4 starter_position, glm::vec4 starter_view_direction);
        /** Default destructor */
        virtual ~enemy();
        glm::vec4 getEnemyToPlayerDirection() { return this->enemy_to_player_direction; }
        void setEnemyToPlayerDirection(glm::vec4 new_enemy_to_player_direction) { this->enemy_to_player_direction = new_enemy_to_player_direction; }
        float getDetectionRadius() {return this->detection_radius ;}

    protected:

    private:
        glm::vec4 enemy_to_player_direction;
        float detection_radius;
};

#endif // ENEMY_H
