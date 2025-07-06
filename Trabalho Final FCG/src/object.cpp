#include "object.h"

object::object(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction){
    setSpeed(speed);
    setPosition(starter_position);
    setScalingFactor(starter_scaling_factor);
    setViewDirection(starter_view_direction);
}

object::~object()
{

}
