//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   LABORATÓRIO 2
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cfloat>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

// Headers da biblioteca para carregar texturas
#include <stb_image.h>

// Headers de classes que representam objetos no jogo
#include "entity.h"
#include "enemy.h"
#include "kraken.h"
#include "squid.h"
#include "player.h"
#include "camera.h"
#include "fireball.h"
#include "dragon.h"
#include "bezierBuilder.h"
#include "rocha.h"
#include "pedestal.h"
#include "cave.h"
#include "ground.h"

// Headers de Funções originalmente definidas pelo professor Eduardo Gastal
#include "funcoes_prof.h"

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = false;

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed;

// Variáveis que determinam se estamos pressionando uma tecla do teclado que movimente a posição c da câmera
struct teclas_pressionadas_para_movimento{
    bool g_wKeyPressed = false;
    bool g_sKeyPressed = false;
    bool g_dKeyPressed = false;
    bool g_aKeyPressed = false;
}typedef MOVEMENT_KEYS_CHECKS;

MOVEMENT_KEYS_CHECKS teclas;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = -3.9f; // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = -0.6f;   // Ângulo em relação ao eixo Y
float g_CameraDistance = 2.5f; // Distância da câmera para a origem

// Variável que controla se estamos jogando em primeira ou terceira pessoa
bool g_CameraFirstPerson = true;

// Variável que determina se o jogador está ou não realizando um ataque
bool is_attacking = false;

int main(int argc, char* argv[])
{

    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 800 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Peasent's Meadow", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Definimos que o mouse não ficará visível na aplicação e que ele não saia da tela enquanto o jogo estiver ativo
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSize(window, 800, 800); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // A cena virtual é uma lista de objetos nomeados, guardados em um dicionário
    // (map).  Veja dentro da função BuildTrianglesAndAddToVirtualScene() como que são incluídos
    // objetos dentro da variável g_VirtualScene, e veja na função main() como
    // estes são acessados.
    std::map<std::string, SceneObject> g_VirtualScene;

    // Pilha que guardará as matrizes de modelagem.
    std::stack<glm::mat4>  g_MatrixStack;

    // VEJA FUNÇÕES_PROF.H PARA DEFINIÇÃO
    SHADERS_DEFINITIONS shaders;

    // Número de texturas carregadas pela função LoadTextureImage()
    GLuint g_NumLoadedTextures = 0;

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles(shaders);

    LoadTextureImage("../../data/Kraken/11.png", g_NumLoadedTextures); // TextureImage0
    LoadTextureImage("../../data/Kraken/12.png", g_NumLoadedTextures); // TextureImage1
    LoadTextureImage("../../data/chevalier/chevalier.bmp", g_NumLoadedTextures); // TextureImage2
    LoadTextureImage("../../data/skybox/cloudy_sky.jpg", g_NumLoadedTextures); // TextureImage3
    LoadTextureImage("../../data/dragon/Kinja_Dragern.png", g_NumLoadedTextures); // TextureImage4
    LoadTextureImage("../../data/fireball/fireball.jpg", g_NumLoadedTextures); // TextureImage5
    LoadTextureImage("../../data/ground/coast_rocks.jpg", g_NumLoadedTextures); // TextureImage6
    LoadTextureImage("../../data/rocha/LargeCaveRock.png", g_NumLoadedTextures); // TextureImage7
    LoadTextureImage("../../data/holder/oldgold.jpg", g_NumLoadedTextures); // TextureImage8
    LoadTextureImage("../../data/caverna/ArchSmall_Base.png", g_NumLoadedTextures); // TextureImage9

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ObjModel krakenmodel("../../data/Kraken/kraken.obj");
    ComputeNormals(&krakenmodel);
    BuildTrianglesAndAddToVirtualScene(&krakenmodel, g_VirtualScene);

    ObjModel chevaliermodel("../../data/chevalier/chevalier.obj");
    ComputeNormals(&chevaliermodel);
    BuildTrianglesAndAddToVirtualScene(&chevaliermodel, g_VirtualScene);

    ObjModel skyboxmodel("../../data/skybox/sphere.obj");
    ComputeNormals(&skyboxmodel);
    // inverte as normais da esfera para servir de skybox (normais vão apontar para "dentro" do modelo)
    for(auto& normal : skyboxmodel.attrib.normals){
        normal = -normal;
    }
    BuildTrianglesAndAddToVirtualScene(&skyboxmodel, g_VirtualScene);

    ObjModel dragonmodel("../../data/dragon/KinjaDragern.obj");
    ComputeNormals(&dragonmodel);
    BuildTrianglesAndAddToVirtualScene(&dragonmodel, g_VirtualScene);

    ObjModel fireballmodel("../../data/fireball/sphere.obj");
    ComputeNormals(&fireballmodel);
    BuildTrianglesAndAddToVirtualScene(&fireballmodel, g_VirtualScene);

    ObjModel groundmodel("../../data/ground/forrestground.obj");
    ComputeNormals(&groundmodel);
    BuildTrianglesAndAddToVirtualScene(&groundmodel, g_VirtualScene);

    ObjModel rockmodel("../../data/rocha/LargeCaveRock.obj");
    ComputeNormals(&rockmodel);
    BuildTrianglesAndAddToVirtualScene(&rockmodel, g_VirtualScene);

    ObjModel pedestalmodel("../../data/holder/objTorch.obj");
    ComputeNormals(&pedestalmodel);
    BuildTrianglesAndAddToVirtualScene(&pedestalmodel, g_VirtualScene);

    ObjModel cavemodel("../../data/caverna/SmallArch.obj");
    ComputeNormals(&cavemodel);
    BuildTrianglesAndAddToVirtualScene(&cavemodel, g_VirtualScene);

    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model, g_VirtualScene);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // inicialização de variáveis que armazenam o tempo a fins de comparação
    double tempo_anterior = glfwGetTime();
    double tempo_atual;
    double deltat;

    // Definição da direção inicial de visualização da câmera
    float r = g_CameraDistance;
    float y = r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);

    // Inicialização da Instancia do jogador controlável. Jogador e câmera devem ter a mesma posição. Jogador deve sempre olhar para a mesma direção da câmera
    STANDARD_PLAYER_ATTRIBUTES atributos_jogador;
                                        // posição inicial  // direção para onde o modelo está olhando  //fator de escalamento
    player jogador(atributos_jogador, glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec4(x,y,z,0.0f), glm::vec3(3.0f,3.0f,3.0f));

    // variáveis para ajudar na lógica de ataque do jogador
    bool first_time_in_attack_loop = true;

    double attack_inicial_time = 0.0;
    fireball *bola_jogador = nullptr;

    // Inicialização da instância da câmera (em primeira pessoa)
        // posição inicial da câmera    // direção para onde a câmera está olhando    // direção para onde a câmera pode "andar" (não pode mudar sua posição vertical relativa ao modelo do jogador)
    camera cam(jogador.getPlayerHeadPosition(), glm::vec4(x,y,z,0.0f), glm::vec4(x,0.0f,z,0.0f));

    // Gravidade do mundo é o oposto do vetor "up" da câmera
    glm::vec4 gravity = -cam.getUpVector();

    // Inicialização dos objetos inanimados:

    // chão
    ground chao(velocidade_objeto_inanimado, glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec3(3.0f,1.0f,3.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    // rochas
    const int total_de_rochas = 5;
    rocha pedra1(velocidade_objeto_inanimado, glm::vec4(6.0f,0.0f,4.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
    //rocha pedra2(velocidade_objeto_inanimado, glm::vec4(12.0f,0.0f,7.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
    //rocha pedra3(velocidade_objeto_inanimado, glm::vec4(1.0f,0.0f,24.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
    //rocha pedra4(velocidade_objeto_inanimado, glm::vec4(2.0f,0.0f,-5.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
    //rocha pedra5(velocidade_objeto_inanimado, glm::vec4(-6.0f,0.0f,-4.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
    rocha pedras[1] = {pedra1};//, pedra2, pedra3, pedra4, pedra5};

    //pedestal
    pedestal pedestal_do_dragao(velocidade_objeto_inanimado, glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    //caverna
    cave caverna(velocidade_objeto_inanimado, glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    // Inicialização dos inimigos
    // lulas
    const int total_de_lulas = 3;
    STANDARD_SQUID_ATTRIBUTES atributos_lula;
    squid lula1(atributos_lula, glm::vec4(3.0f,0.0f,5.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid lula2(atributos_lula, glm::vec4(10.0f,0.0f,1.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid lula3(atributos_lula, glm::vec4(6.0f,0.0f,9.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid instancias_de_lulas[total_de_lulas] = {lula1, lula2, lula3};

    //dragão
    STANDARD_DRAGON_ATTRIBUTES atributos_dragao;
    dragon dragao(atributos_dragao, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) - jogador.getPosition()), glm::vec3(1.0,1.0,1.0));
    std::vector<fireball*> bolas_de_fogo;
    // tesouro que o dragão guarda (vai dar o poder de ataque à distância e permitir matar o chefe)
    fireball *treasure_fireball = new fireball(velocidade_objeto_inanimado, glm::vec4(pedestal_do_dragao.getPosition().x,pedestal_do_dragao.getPosition().y+5.0f,pedestal_do_dragao.getPosition().z,1.0f), glm::vec3(1.5f,1.5f,1.5f), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    //Kraken
    STANDARD_KRAKEN_ATTRIBUTES atributos_kraken;
    kraken chefe(atributos_kraken, glm::vec4(0.0f, 0.0f, 30.0f, 1.0f), glm::vec4(0.0f, 0.0f, 30.0f, 0.0f), glm::vec3(5.0,5.0,5.0));

    // objeto que calcula a curva de bezier para o movimento do dragão
    double t_anterior = 0.0f;
    double t_atual;
    const double duracao_bezier = 5.0;
    bool esta_na_direcao_bezier_original = true;
    bezierBuilder bezier;

    //pontos da curva bezier cúbica do dragão
    glm::vec4 p0(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 p1(2.0f, 5.0f, 0.0f, 1.0f);
    glm::vec4 p2(5.0f, -5.0f, 0.0f, 1.0f);
    glm::vec4 p3(10.0f, 0.0f, 0.0f, 1.0f);

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(shaders.g_GpuProgramID);

        // Computamos o sentido para onde a câmera está olhando utilizando coordenadas esféricas.  As
        // variáveis g_CameraDistance, g_CameraPhi, e g_CameraTheta são
        // controladas pelo mouse do usuário. Veja as funções CursorPosCallback()
        // e ScrollCallback().
        // Câmera não pode se mover enquanto o jogador estiver atacando corpo-a-corpo
        if(!is_attacking){
            r = g_CameraDistance;
            y = r*sin(g_CameraPhi);
            z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
            x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
        }

        // Definição da matriz "View", que determina o sistema de coordenadas da câmera
        glm::mat4 view;

        // cálculo para determinar a atualização de desenhos para animações
        tempo_atual = glfwGetTime();
        deltat = tempo_atual - tempo_anterior;
        tempo_anterior = tempo_atual;

        // "F" para toggle da câmera
        // se escolhemos a câmera em primeira pessoa
        if(g_CameraFirstPerson){
            // Seta a posição a posição da câmera no mesmo local da cabeça do modelo do jogador (no começo, em caso de que a câmera seja trocada de terceira para primeira pessoa)
            cam.setCameraPosition(jogador.getPlayerHeadPosition());

            jogador.setViewDirection(glm::vec4(x,y,z,0.0f)); // Vetor "view" do jogador (para onde o modelo estará olhando)

            //Determinação do vetor view (free camera) da câmera, que determina para onde ela está olhando
            cam.setViewVector(jogador.getViewDirection()); // olha para a mesma direção que o modelo
        // se escolhemos a câmera em terceira pessoa
        } else {
            // Seta a posição da câmera um pouco atrás da direção onde o jogador está olhando (no começo, em caso de que a câmera seja trocada de primeira para terceira pessoa)
            cam.setCameraPosition(glm::vec4(jogador.getPlayerHeadPosition().x-(x*g_CameraDistance),jogador.getPlayerHeadPosition().y-(y*g_CameraDistance),jogador.getPlayerHeadPosition().z-(z*g_CameraDistance),1.0f));

            jogador.setViewDirection(glm::vec4(x,0.0f,z,0.0f)); // Vetor "view" do jogador (para onde o modelo estará olhando)

            //Determinação do vetor view da câmera, que determina para onde ela está olhando (look-at-camera)
            cam.setViewVector(jogador.getPlayerHeadPosition() - cam.getCameraPosition());
        }

        cam.setCameraFowardMoveVector(glm::vec4(x,0.0f,z,0.0f)); // vetor que determina a direção para onde a câmera pode se mexer quando andamos para frente/trás. Impede a câmera de "voar" para longe

        // Determinação dos vetores w e u, que definem o sistema de coordenadas da câmera
        jogador.setPlayerWVector(cam.getCameraFowardMoveVector()/norm(cam.getCameraFowardMoveVector())); // Vetor "w" aponta para "trás" da câmera
        jogador.setPlayerUVector(crossproduct(cam.getUpVector(), jogador.getPlayerWVector())/norm(crossproduct(cam.getUpVector(), jogador.getPlayerWVector()))); // Vetor "u" aponta para "direita" da câmera

        // Normalização dos vetores w e u
        jogador.setPlayerWVector(jogador.getPlayerWVector()/norm(jogador.getPlayerWVector()));
        jogador.setPlayerUVector(jogador.getPlayerUVector()/norm(jogador.getPlayerUVector()));

        // Atualização da posição do personagem controlável. Jogador não pode se mexer se estiver atacando corpo-a-corpo
        if(!is_attacking && teclas.g_wKeyPressed){
            jogador.setPlayerPosition(jogador.getPosition() += scalarMultiplication(jogador.getPlayerWVector(),jogador.getSpeed()*deltat));
        }
        if(!is_attacking && teclas.g_sKeyPressed){
            jogador.setPlayerPosition(jogador.getPosition() -= scalarMultiplication(jogador.getPlayerWVector(),jogador.getSpeed()*deltat));
        }
        if(!is_attacking && teclas.g_dKeyPressed){
            jogador.setPlayerPosition(jogador.getPosition() -= scalarMultiplication(jogador.getPlayerUVector(),jogador.getSpeed()*deltat));
        }
        if(!is_attacking && teclas.g_aKeyPressed){
            jogador.setPlayerPosition(jogador.getPosition() += scalarMultiplication(jogador.getPlayerUVector(),jogador.getSpeed()*deltat));
        }

        // verifica se o jogador fez um ataque
        if(g_LeftMouseButtonPressed){
            g_LeftMouseButtonPressed = false;
            if(jogador.attackIsValid(tempo_atual)){
                is_attacking = true;
                first_time_in_attack_loop = true;
            }
        }
        // segue a lógica de como o ataque deve funcionar, dependendo se o jogador desbloqueou ataques à distância ou não
        if(is_attacking){
            if(!jogador.getHasDragonPower()){
                if(first_time_in_attack_loop){
                    bola_jogador = new fireball(velocidade_objeto_inanimado, jogador.getPlayerHeadPosition() += scalarMultiplication(jogador.getViewDirection(),1.0f), glm::vec3(1.0,1.0,1.0), jogador.getViewDirection());
                    first_time_in_attack_loop = false;
                    attack_inicial_time = glfwGetTime();
                }
                else{
                    if(glfwGetTime() - attack_inicial_time > jogador.getTempoParaJogadorAtacarDeNovo()){
                        bola_jogador = nullptr;
                        is_attacking = false;
                    }
                }
            }
            else{
                if(first_time_in_attack_loop){
                    bola_jogador = new fireball(velocidade_bola_de_fogo_dinamica, jogador.getPlayerHeadPosition() += scalarMultiplication(jogador.getViewDirection(),1.0f), glm::vec3(1.0,1.0,1.0), jogador.getViewDirection());
                    first_time_in_attack_loop = false;
                    attack_inicial_time = glfwGetTime();
                }
                else{
                    if(glfwGetTime() - attack_inicial_time > jogador.getTempoParaJogadorAtacarDeNovo()){
                        is_attacking = false;
                    }
                }
            }
        }
        // movimento da bola de fogo se o jogador tiver desbloqueado ataques à distância. Continua até o jogador realizar um novo ataque (ou até a bola atingir um alvo)
        if(bola_jogador != nullptr){
            bola_jogador->setPosition(bola_jogador->getPosition() += scalarMultiplication(bola_jogador->getViewDirection(),bola_jogador->getSpeed()*deltat));
        }

        // Atualização da posição da câmera de acordo com a visão (primeira ou terceira pessoa)
        if(g_CameraFirstPerson){
            cam.setCameraPosition(jogador.getPlayerHeadPosition());
        } else{
            cam.setCameraPosition(glm::vec4(jogador.getPlayerHeadPosition().x-(x*g_CameraDistance),jogador.getPlayerHeadPosition().y-(y*g_CameraDistance),jogador.getPlayerHeadPosition().z-(z*g_CameraDistance),1.0f));
        }

        view = Matrix_Camera_View(cam.getCameraPosition(), cam.getViewVector(), cam.getUpVector());

        // atualiza a posição das lulas
        // elas andam na direção do jogador se ele estiver próximo delas
        // e voltam para a posição inicial se ele se afastar
        for(squid& instancia_de_lula : instancias_de_lulas){
            instancia_de_lula.setEnemyToPlayerDirection(jogador.getPosition() - instancia_de_lula.getPosition());
            if (norm(jogador.getPosition() - instancia_de_lula.getPosition()) < instancia_de_lula.getDetectionRadius()){
                instancia_de_lula.setPosition(instancia_de_lula.getPosition() += scalarMultiplication(normalize(instancia_de_lula.getEnemyToPlayerDirection()),instancia_de_lula.getSpeed()*deltat));
                instancia_de_lula.setViewDirection(instancia_de_lula.getEnemyToPlayerDirection());
            } else {
                if (!closeEnoughVectors(instancia_de_lula.getPosition(), instancia_de_lula.getFirstSquidPosition())){
                    instancia_de_lula.setPosition(instancia_de_lula.getPosition() += scalarMultiplication(normalize(instancia_de_lula.getFirstSquidPosition() - instancia_de_lula.getPosition()),instancia_de_lula.getSpeed()*deltat));
                    instancia_de_lula.setViewDirection(instancia_de_lula.getFirstSquidPosition() - instancia_de_lula.getPosition());
                }
                else{
                    instancia_de_lula.setViewDirection(instancia_de_lula.getFirstSquidViewDirection());
                }
            }
        }

        // Atualização da posição do dragão
             // tempo_atual % duracao_bezier
        t_atual = fmod(tempo_atual, duracao_bezier) / duracao_bezier;
        // se chegar ao fim do caminho da curva bezier, inverte a direção
        if (t_atual - t_anterior < 0.0f){
            esta_na_direcao_bezier_original = !esta_na_direcao_bezier_original;
        }
        t_anterior = t_atual;
        if(esta_na_direcao_bezier_original){
            dragao.setPosition(bezier.cubicBezier(p0, p1, p2, p3, t_atual));
        }
        else{
            dragao.setPosition(bezier.cubicBezier(p3, p2, p1, p0, t_atual));
        }
        dragao.setViewDirection(jogador.getPosition() - dragao.getPosition());

        // Se o jogador estiver a uma certa distância do dragão, o dragão o atacará com bolas de fogo
        if(norm(jogador.getPosition() - dragao.getPosition()) < dragao.getDetectionRadius()){
            if(dragao.attackIsValid(tempo_atual)){
                fireball *nova_bola = new fireball(velocidade_bola_de_fogo_dinamica, dragao.getPosition(), glm::vec3(1.0,1.0,1.0), dragao.getViewDirection());
                bolas_de_fogo.push_back(nova_bola);
            }
        }

        // movimento das bolas de fogo do dragão
        for(auto bola_de_fogo : bolas_de_fogo){
            bola_de_fogo->setPosition(bola_de_fogo->getPosition() += scalarMultiplication(bola_de_fogo->getViewDirection(),deltat));
        }

        // Agora computamos a matriz de Projeção.
        glm::mat4 projection;

        // Note que, no sistema de coordenadas da câmera, os planos near e far
        // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -150.0f; // Posição do "far plane"

        // Projeção Perspectiva.
        // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

        // Enviamos as matrizes "view" e "projection" para a placa de vídeo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
        // efetivamente aplicadas em todos os pontos.
        glUniformMatrix4fv(shaders.g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(shaders.g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

        // defines para ajudar na associação dos modelos
        #define KRAKEN_HEAD 0
        #define KRAKEN_BODY 1
        #define KRAKEN_EYE  2
        #define CHEVALIER   3
        #define SKYBOX      4
        #define DRAGON      5
        #define FIREBALL    6
        #define GROUND      7
        #define ROCK        8
        #define PEDESTAL    9
        #define CAVE       10

        // FONTE ************************************************
        // conceito auxiliado por chatgpt. Todos os objetos estavam com a primeira textura setada em TextureImage0 até realizarmos esta mudança
        // associando os samplers2D às texturas
        GLint loc_tex0 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage0");
        glUniform1i(loc_tex0, KRAKEN_HEAD); // Unidade GL_TEXTURE0

        GLint loc_tex1 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage1");
        glUniform1i(loc_tex1, KRAKEN_BODY); // Unidade GL_TEXTURE1

        GLint loc_tex2 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage2");
        glUniform1i(loc_tex2, KRAKEN_EYE); // Unidade GL_TEXTURE2

        GLint loc_tex3 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage3");
        glUniform1i(loc_tex3, CHEVALIER); // Unidade GL_TEXTURE3

        GLint loc_tex4 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage4");
        glUniform1i(loc_tex4, SKYBOX); // Unidade GL_TEXTURE4

        GLint loc_tex5 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage5");
        glUniform1i(loc_tex5, DRAGON); // Unidade GL_TEXTURE5

        GLint loc_tex6 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage6");
        glUniform1i(loc_tex6, FIREBALL); // Unidade GL_TEXTURE6

        GLint loc_tex7 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage7");
        glUniform1i(loc_tex7, GROUND); // Unidade GL_TEXTURE7

        GLint loc_tex8 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage8");
        glUniform1i(loc_tex8, ROCK); // Unidade GL_TEXTURE8

        GLint loc_tex9 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage9");
        glUniform1i(loc_tex9, PEDESTAL); // Unidade GL_TEXTURE9

        GLint loc_tex10 = glGetUniformLocation(shaders.g_GpuProgramID, "TextureImage10");
        glUniform1i(loc_tex10, CAVE); // Unidade GL_TEXTURE10
        // FONTE ************************************************

        // desenhamos a esfera que serve como skybox
        // Precisamos pedir para que o opengl faça o culling das faces externas da esfera
        glCullFace(GL_FRONT);
        model = Matrix_Translate(jogador.getPosition().x,jogador.getPosition().y,jogador.getPosition().z) // Skybox fica relativa ao jogador
              * Matrix_Scale(100.0f, 100.0f, 100.0f);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, SKYBOX);
        DrawVirtualObject("the_sphere", g_VirtualScene);
        // Fazemos o culling das faces internas dos outros objetos
        glCullFace(GL_BACK);

        for(squid& instancia_de_lula : instancias_de_lulas){
            // desenhamos a "cabeça" das lulas
            model = Matrix_Translate(instancia_de_lula.getPosition().x,instancia_de_lula.getPosition().y,instancia_de_lula.getPosition().z)
                  * Matrix_Scale(instancia_de_lula.getScalingFactor().x,instancia_de_lula.getScalingFactor().y,instancia_de_lula.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(instancia_de_lula.getViewDirection().x, instancia_de_lula.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_HEAD);
            DrawVirtualObject("Sub-Model_0.001", g_VirtualScene);

            // desenhamos o "corpo" das lulas
            model = Matrix_Translate(instancia_de_lula.getPosition().x,instancia_de_lula.getPosition().y,instancia_de_lula.getPosition().z)
                  * Matrix_Scale(instancia_de_lula.getScalingFactor().x,instancia_de_lula.getScalingFactor().y,instancia_de_lula.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(instancia_de_lula.getViewDirection().x, instancia_de_lula.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_BODY);
            DrawVirtualObject("Sub-Model_0", g_VirtualScene);

            // Desenhamos o olho das lulas
            model = Matrix_Translate(instancia_de_lula.getPosition().x,instancia_de_lula.getPosition().y,instancia_de_lula.getPosition().z)
                  * Matrix_Scale(instancia_de_lula.getScalingFactor().x,instancia_de_lula.getScalingFactor().y,instancia_de_lula.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(instancia_de_lula.getViewDirection().x, instancia_de_lula.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_EYE);
            DrawVirtualObject("Sub-Model_1", g_VirtualScene);
        }

        // Desenhamos o personagem jogável
        model = Matrix_Translate(jogador.getPosition().x,jogador.getPosition().y,jogador.getPosition().z)
              * Matrix_Scale(jogador.getScalingFactor().x,jogador.getScalingFactor().y,jogador.getScalingFactor().z)
              * Matrix_Rotate_Y(atan2(jogador.getViewDirection().x, jogador.getViewDirection().z));
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, CHEVALIER);
        DrawVirtualObject("chevalier", g_VirtualScene);

        // Desenhamos o Dragão
        model = Matrix_Translate(dragao.getPosition().x,dragao.getPosition().y,dragao.getPosition().z)
              * Matrix_Scale(dragao.getScalingFactor().x,dragao.getScalingFactor().y,dragao.getScalingFactor().z)
              * Matrix_Rotate_Y(atan2(dragao.getViewDirection().x, dragao.getViewDirection().z));
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, DRAGON);
        DrawVirtualObject("KinjaDragern", g_VirtualScene);

        // Desenhamos o ataque do jogador, se existir
        if(bola_jogador != nullptr){
            model = Matrix_Translate(bola_jogador->getPosition().x,bola_jogador->getPosition().y,bola_jogador->getPosition().z);
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, FIREBALL);
            DrawVirtualObject("sphere", g_VirtualScene);
        }

        // Desenhamos as bolas de fogo do dragão
        for(auto bola_de_fogo : bolas_de_fogo){
            model = Matrix_Translate(bola_de_fogo->getPosition().x,bola_de_fogo->getPosition().y,bola_de_fogo->getPosition().z);
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, FIREBALL);
            DrawVirtualObject("sphere", g_VirtualScene);
        }

        // Desenhamos o chão
        model = Matrix_Translate(chao.getPosition().x,chao.getPosition().y,chao.getPosition().z)
              * Matrix_Scale(chao.getScalingFactor().x,chao.getScalingFactor().y,chao.getScalingFactor().z);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, GROUND);
        DrawVirtualObject("groundAlow", g_VirtualScene);

        // Desenhamos o Kraken:
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // desenhamos a "cabeça" do kraken
            model = Matrix_Translate(chefe.getPosition().x,chefe.getPosition().y,chefe.getPosition().z)
                  * Matrix_Scale(chefe.getScalingFactor().x,chefe.getScalingFactor().y,chefe.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(chefe.getViewDirection().x, chefe.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_HEAD);
            DrawVirtualObject("Sub-Model_0.001", g_VirtualScene);

            // desenhamos o "corpo" do kraken
            model = Matrix_Translate(chefe.getPosition().x,chefe.getPosition().y,chefe.getPosition().z)
                  * Matrix_Scale(chefe.getScalingFactor().x,chefe.getScalingFactor().y,chefe.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(chefe.getViewDirection().x, chefe.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_BODY);
            DrawVirtualObject("Sub-Model_0", g_VirtualScene);

            // Desenhamos o olho do kraken
            model = Matrix_Translate(chefe.getPosition().x,chefe.getPosition().y,chefe.getPosition().z)
                  * Matrix_Scale(chefe.getScalingFactor().x,chefe.getScalingFactor().y,chefe.getScalingFactor().z)
                  * Matrix_Rotate_Y(atan2(chefe.getViewDirection().x, chefe.getViewDirection().z));
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, KRAKEN_EYE);
            DrawVirtualObject("Sub-Model_1", g_VirtualScene);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        // desenhamos as rochas:
        /*for(auto& pedra : pedras){
            model = Matrix_Translate(pedra.getPosition().x,pedra.getPosition().y,pedra.getPosition().z)
                  * Matrix_Scale(pedra.getScalingFactor().x,pedra.getScalingFactor().y,pedra.getScalingFactor().z);
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, ROCK);
            DrawVirtualObject("LargeCaveRock", g_VirtualScene);
        }*/

        // desenhamos o pedestal:
        model = Matrix_Translate(pedestal_do_dragao.getPosition().x,pedestal_do_dragao.getPosition().y,pedestal_do_dragao.getPosition().z)
                * Matrix_Scale(pedestal_do_dragao.getScalingFactor().x,pedestal_do_dragao.getScalingFactor().y,pedestal_do_dragao.getScalingFactor().z);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, PEDESTAL);
        DrawVirtualObject("group torch", g_VirtualScene);

        // Desenhamos a bola de fogo que fica em cima do pedestal, se ela não foi coletada
        model = Matrix_Translate(treasure_fireball->getPosition().x,treasure_fireball->getPosition().y,treasure_fireball->getPosition().z);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, FIREBALL);
        DrawVirtualObject("sphere", g_VirtualScene);

        // desenhamos a caverna:
        model = Matrix_Translate(caverna.getPosition().x,caverna.getPosition().y,caverna.getPosition().z)
                * Matrix_Scale(caverna.getScalingFactor().x,caverna.getScalingFactor().y,caverna.getScalingFactor().z);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, CAVE);
        DrawVirtualObject("Arch Small", g_VirtualScene);

        // Imprimimos na tela informação sobre o número de quadros renderizados
        // por segundo (frames per second).
        if(g_ShowInfoText){
            TextRendering_ShowFramesPerSecond(window, g_ShowInfoText);
        }

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usuário
        // tudo que foi renderizado pelas funções acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
        // definidas anteriormente usando glfwSet*Callback() serão chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse setamos a variável
        // g_LeftMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Abaixo executamos o seguinte: computamos quanto que o mouse se movimento desde o último
    // instante de tempo, e usamos esta movimentação para atualizar os
    // parâmetros que definem a posição da câmera dentro da cena virtual.
    // Assim, temos que o usuário consegue controlar a câmera.

    // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
    if(!is_attacking){
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;


        // Atualizamos parâmetros da câmera com os deslocamentos
        g_CameraTheta -= 0.01f*dx;
        g_CameraPhi   -= 0.01f*dy;

        // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2;
        float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Atualizamos a distância da câmera para a origem utilizando a
    // movimentação da "rodinha", simulando um ZOOM.
    // Mas apenas quando estivermos em terceira pessoa
    if(!g_CameraFirstPerson){
        g_CameraDistance -= 0.1f*yoffset;
    }

    // Uma câmera look-at nunca pode estar exatamente "em cima" do ponto para
    // onde ela está olhando, pois isto gera problemas de divisão por zero na
    // definição do sistema de coordenadas da câmera. Isto é, a variável abaixo
    // nunca pode ser zero. Versões anteriores deste código possuíam este bug,
    // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (g_CameraDistance < verysmallnumber)
        g_CameraDistance = verysmallnumber;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Se o usuário apertar a tecla F, fazemos um "toggle" entre primeira e terceira pessoa.
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        g_CameraFirstPerson = !g_CameraFirstPerson;
    }

    // Se o usuário apertar a tecla W, movemos a posição da câmera para "frente"
    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS){
            teclas.g_wKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_wKeyPressed = false;
        }
    }

    // Se o usuário apertar a tecla S, movemos a posição da câmera para "trás"
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS){
            teclas.g_sKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_sKeyPressed = false;
        }
    }

    // Se o usuário apertar a tecla D, movemos a posição da câmera para "direita"
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS){
            teclas.g_dKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_dKeyPressed = false;
        }
    }

    // Se o usuário apertar a tecla A, movemos a posição da câmera para "esquerda"
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS){
            teclas.g_aKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_aKeyPressed = false;
        }
    }

    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :



