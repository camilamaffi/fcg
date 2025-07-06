#include "pedestal.h"

pedestal::pedestal(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction) : object(speed, starter_position, starter_scaling_factor, starter_view_direction)
{
};

pedestal::~pedestal()
{
    //dtor
}
