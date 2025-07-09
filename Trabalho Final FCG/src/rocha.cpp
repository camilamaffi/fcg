#include "rocha.h"

rocha::rocha(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto) : object(speed, starter_position, starter_scaling_factor, starter_view_direction, objeto)
{
};

rocha::~rocha()
{
    //dtor
}
