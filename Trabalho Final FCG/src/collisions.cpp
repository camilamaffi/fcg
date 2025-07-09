#include "collisions.h"

bool boxBoxCollision(glm::vec4 bbox_min1, glm::vec4 bbox_max1, glm::vec4 bbox_min2, glm::vec4 bbox_max2){
    return (bbox_min1.x <= bbox_max2.x && bbox_max1.x >= bbox_min2.x) && (bbox_min1.y <= bbox_max2.y && bbox_max1.y >= bbox_min2.y) && (bbox_min1.z <= bbox_max2.z && bbox_max1.z >= bbox_min2.z);
}

// Usada apenas para verificar se um objeto ao qual deve ser aplicada gravidade está colidindo com o chão
bool boxPlaneCollision(glm::vec4 bbox_min, glm::vec4 bbox_max, glm::vec4 bplane_min, glm::vec4 bplane_max){
    return !((bbox_min.x >= bplane_max.x && bbox_max.x >= bplane_max.x) || (bbox_min.x <= bplane_min.x && bbox_max.x <= bplane_min.x)) && (bbox_min.y <= bplane_max.y && bbox_max.y >= bplane_min.y) && !((bbox_min.z >= bplane_max.z && bbox_max.z >= bplane_max.z) || (bbox_min.z <= bplane_min.z && bbox_max.z <= bplane_min.z));
}

//FONTE*********************************************************************************************
// Testes de colisão esfera-caixa e esfera-"plano" auxiliados por chatgpt
// No fim acabei implementando um plano como uma caixa sem dimensão y, mas deixei as duas funções aqui por questões de documentação
bool boxSphereCollision(glm::vec4 bbox_min, glm::vec4 bbox_max, glm::vec4 centro, float raio){
    float x = std::max(bbox_min.x, std::min(centro.x, bbox_max.x));
    float y = std::max(bbox_min.y, std::min(centro.y, bbox_max.y));
    float z = std::max(bbox_min.z, std::min(centro.z, bbox_max.z));

    float dx = x - centro.x;
    float dy = y - centro.y;
    float dz = z - centro.z;

    float distanceSquared = dx*dx + dy*dy + dz*dz;
    return distanceSquared <= (raio * raio);
}

bool spherePlaneCollision(glm::vec4 centro, float raio, glm::vec4 bplane_min, glm::vec4 bplane_max){
    float x = std::max(bplane_min.x, std::min(centro.x, bplane_max.x));
    float y = std::max(bplane_min.y, std::min(centro.y, bplane_max.y));
    float z = std::max(bplane_min.z, std::min(centro.z, bplane_max.z));

    float dx = x - centro.x;
    float dy = y - centro.y;
    float dz = z - centro.z;

    float distanceSquared = dx*dx + dy*dy + dz*dz;
    return distanceSquared <= (raio * raio);
}
//FONTE*********************************************************************************************

bool sphereSphereCollision(glm::vec4 centro1, float raio1, glm::vec4 centro2, float raio2){
    return (norm(centro1 - centro2) <= raio2+raio1);
}

bool playerCanMove(player jogador, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna){
    bool podeSeMover = true;
    podeSeMover = podeSeMover && !boxSphereCollision(chefe.getTransformedBboxMin(), chefe.getTransformedBboxMax(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());
    for(auto lula : lulas){
        podeSeMover = podeSeMover && !sphereSphereCollision(jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio(), lula->getTransformedBsphereCentro(), lula->getTransformedBsphereRaio());
    }
    //podeSeMover = podeSeMover && !boxSphereCollision(dragao.getTransformedBboxMin(), dragao.getTransformedBboxMax(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());
    for(auto& pedra : pedras){
        podeSeMover = podeSeMover && !boxSphereCollision(pedra.getTransformedBboxMin(), pedra.getTransformedBboxMax(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !boxSphereCollision(pedestal_do_dragao.getTransformedBboxMin(), pedestal_do_dragao.getTransformedBboxMax(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());
    //podeSeMover = podeSeMover && !boxSphereCollision(caverna.getTransformedBboxMin(), caverna.getTransformedBboxMax(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());

    return podeSeMover;
}

bool squidCanMove(squid& lula_testada, player jogador, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna){
    bool podeSeMover = true;
    podeSeMover = podeSeMover && !sphereSphereCollision(lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio(), jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio());
    podeSeMover = podeSeMover && !boxSphereCollision(chefe.getTransformedBboxMin(), chefe.getTransformedBboxMax(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());
    for(auto lula : lulas){
        if(&lula_testada != lula){
            podeSeMover = podeSeMover && !sphereSphereCollision(lula->getTransformedBsphereCentro(), lula->getTransformedBsphereRaio(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());
        }
    }
    podeSeMover = podeSeMover && !boxSphereCollision(dragao.getTransformedBboxMin(), dragao.getTransformedBboxMax(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());
    for(auto& pedra : pedras){
        podeSeMover = podeSeMover && !boxSphereCollision(pedra.getTransformedBboxMin(), pedra.getTransformedBboxMax(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !boxSphereCollision(pedestal_do_dragao.getTransformedBboxMin(), pedestal_do_dragao.getTransformedBboxMax(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());
    //podeSeMover = podeSeMover && !boxSphereCollision(caverna.getTransformedBboxMin(), caverna.getTransformedBboxMax(), lula_testada.getTransformedBsphereCentro(), lula_testada.getTransformedBsphereRaio());

    return podeSeMover;
}

bool playerFireballCanMove(fireball& bola_de_fogo, kraken chefe, std::vector<squid*> lulas, dragon dragao, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna, ground chao){
    bool podeSeMover = true;
    podeSeMover = podeSeMover && !boxSphereCollision(chefe.getTransformedBboxMin(), chefe.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    for(auto lula : lulas){
        podeSeMover = podeSeMover && !sphereSphereCollision(lula->getTransformedBsphereCentro(), lula->getTransformedBsphereRaio(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !boxSphereCollision(dragao.getTransformedBboxMin(), dragao.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    for(auto& pedra : pedras){
        podeSeMover = podeSeMover && !boxSphereCollision(pedra.getTransformedBboxMin(), pedra.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !boxSphereCollision(pedestal_do_dragao.getTransformedBboxMin(), pedestal_do_dragao.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    //podeSeMover = podeSeMover && !boxSphereCollision(caverna.getTransformedBboxMin(), caverna.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    podeSeMover = podeSeMover && !spherePlaneCollision(bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio(), chao.getTransformedBplaneMin(),  chao.getTransformedBplaneMax());
    return podeSeMover;
}

bool dragonFireballCanMove(fireball& bola_de_fogo, kraken chefe, std::vector<squid*> lulas, player jogador, std::vector<rocha> pedras, pedestal pedestal_do_dragao, cave caverna, ground chao){
    bool podeSeMover = true;
    podeSeMover = podeSeMover && !boxSphereCollision(chefe.getTransformedBboxMin(), chefe.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    for(auto lula : lulas){
        podeSeMover = podeSeMover && !sphereSphereCollision(lula->getTransformedBsphereCentro(), lula->getTransformedBsphereRaio(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !sphereSphereCollision(jogador.getTransformedBsphereCentro(), jogador.getTransformedBsphereRaio(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    for(auto& pedra : pedras){
        podeSeMover = podeSeMover && !boxSphereCollision(pedra.getTransformedBboxMin(), pedra.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    }
    podeSeMover = podeSeMover && !boxSphereCollision(pedestal_do_dragao.getTransformedBboxMin(), pedestal_do_dragao.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    podeSeMover = podeSeMover && !boxSphereCollision(caverna.getTransformedBboxMin(), caverna.getTransformedBboxMax(), bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio());
    podeSeMover = podeSeMover && !spherePlaneCollision(bola_de_fogo.getTransformedBsphereCentro(), bola_de_fogo.getTransformedBsphereRaio(), chao.getTransformedBplaneMin(),  chao.getTransformedBplaneMax());
    return podeSeMover;
}


