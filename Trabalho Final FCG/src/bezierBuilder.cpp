#include "bezierBuilder.h"

bezierBuilder::bezierBuilder(){

}

bezierBuilder::~bezierBuilder(){

}

// FONTE *************************************************************************************************************************
// Função de Bezier cúbica eficiente auxiliada por chatgpt
glm::vec4 bezierBuilder::cubicBezier(const glm::vec4& p0, const glm::vec4& p1, const glm::vec4& p2, const glm::vec4& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec4 point = uuu * p0; // (1 - t)^3 * P0
    point += 3 * uu * t * p1;   // 3(1 - t)^2 * t * P1
    point += 3 * u * tt * p2;   // 3(1 - t) * t^2 * P2
    point += ttt * p3;          // t^3 * P3

    return point;
}
// FONTE *************************************************************************************************************************
