#ifndef OBJECT_H
#define OBJECT_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

const float velocidade_objeto_inanimado = 0.0f;

class object{
    public:
        object(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction);
        ~object();

        float getSpeed() { return speed; }
        void setSpeed(float speed) { this->speed = speed; }

        glm::vec4 getPosition() { return this->position; }
        void setPosition(glm::vec4 new_position) { this->position = new_position; }

        glm::vec3 getScalingFactor() { return this->scaling_factor; }
        void setScalingFactor(glm::vec3 new_scaling_factor) { this->scaling_factor = new_scaling_factor; }

        glm::vec4 getViewDirection() { return this->view_direction; }
        void setViewDirection(glm::vec4 new_view_direction) { this->view_direction = new_view_direction; }

    private:
        float speed;
        glm::vec3 scaling_factor;
        glm::vec4 position;
        glm::vec4 view_direction;
};

#endif
