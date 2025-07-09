#include "bounding_plane.h"

bounding_plane::bounding_plane(glm::vec3 bbox_min, glm::vec3 bbox_max){
    this->bplane_min = glm::vec3(bbox_min.x, (bbox_max.y+bbox_min.y)/2.0f, bbox_min.z);
    this->bplane_max = glm::vec3(bbox_max.x, (bbox_max.y+bbox_min.y)/2.0f, bbox_max.z);
}
