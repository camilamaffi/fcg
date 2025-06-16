#include "camera.h"

camera::camera(glm::vec4 camera_position, glm::vec4 view_vector, glm::vec4 camera_foward_move_vector)
{
    setCameraPosition(camera_position);
    setViewVector(view_vector);
    setCameraFowardMoveVector(camera_foward_move_vector);
}

camera::~camera()
{
    //dtor
}
