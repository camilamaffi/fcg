#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bounding_box.h"
#include "bounding_sphere.h"
#include "bounding_plane.h"

#include "matrices.h"

#include <vector>

// Headers de classes que representam objetos no jogo
#include "entity.h"
#include "enemy.h"
#include "kraken.h"
#include "squid.h"
#include "player.h"
#include "camera.h"
#include "fireball.h"
#include "dragon.h"
#include "rocha.h"
#include "pedestal.h"
#include "cave.h"
#include "ground.h"

bool boxBoxCollision(glm::vec4 bbox_min1, glm::vec4 bbox_max1, glm::vec4 bbox_min2, glm::vec4 bbox_max2);

bool boxPlaneCollision(glm::vec4 bbox_min, glm::vec4 bbox_max, glm::vec4 bplane_min, glm::vec4 bplane_max);

bool boxSphereCollision(glm::vec4 bbox_min, glm::vec4 bbox_max, glm::vec4 centro, float raio);

bool spherePlaneCollision(glm::vec4 centro, float raio, glm::vec4 bplane_min, glm::vec4 bplane_max);

bool sphereSphereCollision(glm::vec4 centro1, float raio1, glm::vec4 centro2, float raio2);

bool playerCanMove(player jogador, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna);

bool squidCanMove(squid& lula_testada, player jogador, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna);

bool playerFireballCanMove(fireball& bola_de_fogo, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna, ground chao);

bool dragonFireballCanMove(fireball& bola_de_fogo, kraken chefe, std::vector<squid*> lulas, player jogador, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna, ground chao);

#endif // COLLISIONS_H
