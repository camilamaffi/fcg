#include "bounding_box.h"

bounding_box::bounding_box(glm::vec3 bbox_min, glm::vec3 bbox_max){
    this->bbox_max = bbox_max;
    this->bbox_min = bbox_min;
}
