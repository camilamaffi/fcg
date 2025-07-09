#include "object.h"

object::object(float speed, glm::vec4 starter_position, glm::vec3 starter_scaling_factor, glm::vec4 starter_view_direction, SceneObject objeto){
    setSpeed(speed);
    setPosition(starter_position);
    setScalingFactor(starter_scaling_factor);
    setViewDirection(starter_view_direction);
    updateModelMatrix(starter_position, starter_scaling_factor, starter_view_direction);
    this->caixa = new bounding_box(objeto.bbox_min, objeto.bbox_max);
    this->esfera = new bounding_sphere(objeto.bbox_min, objeto.bbox_max);
    this->plano = new bounding_plane(objeto.bbox_min, objeto.bbox_max);
}

object::~object()
{
    delete this->caixa;
    delete this->esfera;
    delete this->plano;
}

void object::updateModelMatrix(glm::vec4 position, glm::vec3 scaling_factor, glm::vec4 view_direction){
    this->model = Matrix_Translate(position.x,position.y,position.z)
                  * Matrix_Scale(scaling_factor.x,scaling_factor.y,scaling_factor.z)
                  * Matrix_Rotate_Y(atan2(view_direction.x, view_direction.z));
}
