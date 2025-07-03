#ifndef ENTITY_H
#define ENTITY_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class entity
{
    public:
        /** Default constructor */
        entity(int health_points, int attack_power, int damage_resistance, float speed, glm::vec4 starter_position, glm::vec4 view_direction);
        /** Default destructor */
        virtual ~entity();

        /** Access health_points
         * \return The current value of health_points
         */
        int getHealthPoints() { return health_points; }
        /** Set health_points
         * \param health_points New value to set
         */
        void setHealthPoints(int health_points) { this->health_points = health_points; }
        /** Access attack_power
         * \return The current value of attack_power
         */
        int getAttackPower() { return attack_power; }
        /** Set attack_power
         * \param attack_power New value to set
         */
        void setAttackPower(int attack_power) { this->attack_power = attack_power; }
        /** Access damage_resistance
         * \return The current value of damage_resistance
         */
        int getDamageResistance() { return damage_resistance; }
        /** Set damage_resistance
         * \param damage_resistance New value to set
         */
        void setDamageResistance(int damage_resistance) { this->damage_resistance = damage_resistance; }
        /** Access speed
         * \return The current value of speed
         */
        float getSpeed() { return speed; }
        /** Set speed
         * \param damage_resistance New value to set
         */
        void setSpeed(float speed) { this->speed = speed; }

        glm::vec4 getPosition() { return this->position; }
        void setPosition(glm::vec4 new_position) { this->position = new_position; }

        glm::vec4 getViewDirection() { return this->view_direction; }
        void setViewDirection(glm::vec4 new_view_direction) { this->view_direction = new_view_direction; }

    protected:

    private:
        int health_points; //!< Member variable "health_points"
        int attack_power; //!< Member variable "attack_power"
        int damage_resistance; //!< Member variable "damage_resistance"
        float speed; //!< Member variable "speed"
        glm::vec4 position;
        glm::vec4 view_direction;
};

#endif // ENTITY_H
