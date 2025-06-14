#ifndef ENTITY_H
#define ENTITY_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class entity
{
    public:
        /** Default constructor */
        entity(int health_points, int attack_power, int damage_resistance, float speed);
        /** Default destructor */
        virtual ~entity();

        /** Access health_points
         * \return The current value of health_points
         */
        int Gethealth_points() { return health_points; }
        /** Set health_points
         * \param val New value to set
         */
        void Sethealth_points(int val) { health_points = val; }
        /** Access attack_power
         * \return The current value of attack_power
         */
        int Getattack_power() { return attack_power; }
        /** Set attack_power
         * \param val New value to set
         */
        void Setattack_power(int val) { attack_power = val; }
        /** Access damage_resistance
         * \return The current value of damage_resistance
         */
        int Getdamage_resistance() { return damage_resistance; }
        /** Set damage_resistance
         * \param val New value to set
         */
        void Setdamage_resistance(int val) { damage_resistance = val; }
        /** Access speed
         * \return The current value of speed
         */
        float Getspeed() { return speed; }
        /** Set speed
         * \param val New value to set
         */
        void Setspeed(float val) { speed = val; }

    protected:

    private:
        int health_points; //!< Member variable "health_points"
        int attack_power; //!< Member variable "attack_power"
        int damage_resistance; //!< Member variable "damage_resistance"
        float speed; //!< Member variable "speed"

};

#endif // ENTITY_H
