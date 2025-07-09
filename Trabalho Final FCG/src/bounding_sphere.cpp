#include "bounding_sphere.h"

bounding_sphere::bounding_sphere(glm::vec3 bbox_min, glm::vec3 bbox_max){
    this->centro = glm::vec4((bbox_max.x + bbox_min.x)/2.0f, (bbox_max.y + bbox_min.y)/2.0f, (bbox_max.z + bbox_min.z)/2.0f,1.0f);
    this->raio = std::max(std::max(norm(glm::vec4(bbox_max.x, (bbox_max.y + bbox_min.y)/2.0f, bbox_max.z, 1.0f) - centro), norm(glm::vec4(bbox_min.x, (bbox_max.y + bbox_min.y)/2.0f, bbox_max.z, 1.0f) - centro)), std::max(norm(glm::vec4(bbox_max.x, (bbox_max.y + bbox_min.y)/2.0f, bbox_min.z, 1.0f) - centro), norm(glm::vec4(bbox_min.x, (bbox_max.y + bbox_min.y)/2.0f, bbox_min.z, 1.0f) - centro)));
}
