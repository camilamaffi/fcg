//     Universidade Federal do Rio Grande do Sul
//             Instituto de Inform�tica
//       Departamento de Inform�tica Aplicada
//
//    INF01047 Fundamentos de Computa��o Gr�fica
//               Prof. Eduardo Gastal
//
//                   LABORAT�RIO 2
//

// Arquivos "headers" padr�es de C podem ser inclu�dos em um
// programa C++, sendo necess�rio somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cfloat>

// Headers abaixo s�o espec�ficos de C++
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
#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Cria��o de janelas do sistema operacional

// Headers da biblioteca GLM: cria��o de matrizes e vetores.
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

// Headers de Fun��es originalmente definidas pelo professor Eduardo Gastal
#include "funcoes_prof.h"

// Fun��es callback para comunica��o com o sistema operacional e intera��o do
// usu�rio. Veja mais coment�rios nas defini��es das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// Vari�vel que controla se o texto informativo ser� mostrado na tela.
bool g_ShowInfoText = false;

// Vari�veis globais que armazenam a �ltima posi��o do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// "g_LeftMouseButtonPressed = true" se o usu�rio est� com o bot�o esquerdo do mouse
// pressionado no momento atual. Veja fun��o MouseButtonCallback().
bool g_LeftMouseButtonPressed;

// Vari�veis que determinam se estamos pressionando uma tecla do teclado que movimente a posi��o c da c�mera
struct teclas_pressionadas_para_movimento{
    bool g_wKeyPressed = false;
    bool g_sKeyPressed = false;
    bool g_dKeyPressed = false;
    bool g_aKeyPressed = false;
}typedef MOVEMENT_KEYS_CHECKS;

MOVEMENT_KEYS_CHECKS teclas;

// Raz�o de propor��o da janela (largura/altura). Veja fun��o FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Vari�veis que definem a c�mera em coordenadas esf�ricas, controladas pelo
// usu�rio atrav�s do mouse (veja fun��o CursorPosCallback()). A posi��o
// efetiva da c�mera � calculada dentro da fun��o main(), dentro do loop de
// renderiza��o.
float g_CameraTheta = -3.9f; // �ngulo no plano ZX em rela��o ao eixo Z
float g_CameraPhi = -0.6f;   // �ngulo em rela��o ao eixo Y
float g_CameraDistance = 2.5f; // Dist�ncia da c�mera para a origem

// Vari�vel que controla se estamos jogando em primeira ou terceira pessoa
bool g_CameraFirstPerson = true;

// Vari�vel que determina se o jogador est� ou n�o realizando um ataque
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

    // Definimos o callback para impress�o de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL vers�o 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto �, utilizaremos somente as
    // fun��es modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 800 linhas
    // de pixels, e com t�tulo "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "Peasent's Meadow", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a fun��o de callback que ser� chamada sempre que o usu�rio
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os bot�es do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Definimos que o mouse n�o ficar� vis�vel na aplica��o e que ele n�o saia da tela enquanto o jogo estiver ativo
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Definimos a fun��o de callback que ser� chamada sempre que a janela for
    // redimensionada, por consequ�ncia alterando o tamanho do "framebuffer"
    // (regi�o de mem�ria onde s�o armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSize(window, 800, 800); // For�amos a chamada do callback acima, para definir g_ScreenRatio.

    // Indicamos que as chamadas OpenGL dever�o renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas fun��es definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Imprimimos no terminal informa��es sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // A cena virtual � uma lista de objetos nomeados, guardados em um dicion�rio
    // (map).  Veja dentro da fun��o BuildTrianglesAndAddToVirtualScene() como que s�o inclu�dos
    // objetos dentro da vari�vel g_VirtualScene, e veja na fun��o main() como
    // estes s�o acessados.
    std::map<std::string, SceneObject> g_VirtualScene;

    // Pilha que guardar� as matrizes de modelagem.
    std::stack<glm::mat4>  g_MatrixStack;

    // VEJA FUN��ES_PROF.H PARA DEFINI��O
    SHADERS_DEFINITIONS shaders;

    // N�mero de texturas carregadas pela fun��o LoadTextureImage()
    GLuint g_NumLoadedTextures = 0;

    // Carregamos os shaders de v�rtices e de fragmentos que ser�o utilizados
    // para renderiza��o. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
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

    // Constru�mos a representa��o de objetos geom�tricos atrav�s de malhas de tri�ngulos
    ObjModel krakenmodel("../../data/Kraken/kraken.obj");
    ComputeNormals(&krakenmodel);
    BuildTrianglesAndAddToVirtualScene(&krakenmodel, g_VirtualScene);

    ObjModel chevaliermodel("../../data/chevalier/chevalier.obj");
    ComputeNormals(&chevaliermodel);
    BuildTrianglesAndAddToVirtualScene(&chevaliermodel, g_VirtualScene);

    ObjModel skyboxmodel("../../data/skybox/sphere.obj");
    ComputeNormals(&skyboxmodel);
    // inverte as normais da esfera para servir de skybox (normais v�o apontar para "dentro" do modelo)
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

    // Inicializamos o c�digo para renderiza��o de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // inicializa��o de vari�veis que armazenam o tempo a fins de compara��o
    double tempo_anterior = glfwGetTime();
    double tempo_atual;
    double deltat;

    // Defini��o da dire��o inicial de visualiza��o da c�mera
    float r = g_CameraDistance;
    float y = r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);

    // Inicializa��o da Instancia do jogador control�vel. Jogador e c�mera devem ter a mesma posi��o. Jogador deve sempre olhar para a mesma dire��o da c�mera
    STANDARD_PLAYER_ATTRIBUTES atributos_jogador;
                                        // posi��o inicial  // dire��o para onde o modelo est� olhando  //fator de escalamento
    player jogador(atributos_jogador, glm::vec4(0.0f,0.0f,0.0f,1.0f), glm::vec4(x,y,z,0.0f), glm::vec3(3.0f,3.0f,3.0f));

    // vari�veis para ajudar na l�gica de ataque do jogador
    bool first_time_in_attack_loop = true;

    double attack_inicial_time = 0.0;
    fireball *bola_jogador = nullptr;

    // Inicializa��o da inst�ncia da c�mera (em primeira pessoa)
        // posi��o inicial da c�mera    // dire��o para onde a c�mera est� olhando    // dire��o para onde a c�mera pode "andar" (n�o pode mudar sua posi��o vertical relativa ao modelo do jogador)
    camera cam(jogador.getPlayerHeadPosition(), glm::vec4(x,y,z,0.0f), glm::vec4(x,0.0f,z,0.0f));

    // Gravidade do mundo � o oposto do vetor "up" da c�mera
    glm::vec4 gravity = -cam.getUpVector();

    // Inicializa��o dos objetos inanimados:

    // ch�o
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

    // Inicializa��o dos inimigos
    // lulas
    const int total_de_lulas = 3;
    STANDARD_SQUID_ATTRIBUTES atributos_lula;
    squid lula1(atributos_lula, glm::vec4(3.0f,0.0f,5.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid lula2(atributos_lula, glm::vec4(10.0f,0.0f,1.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid lula3(atributos_lula, glm::vec4(6.0f,0.0f,9.0f,1.0f), glm::vec4(1.0f,0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));
    squid instancias_de_lulas[total_de_lulas] = {lula1, lula2, lula3};

    //drag�o
    STANDARD_DRAGON_ATTRIBUTES atributos_dragao;
    dragon dragao(atributos_dragao, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) - jogador.getPosition()), glm::vec3(1.0,1.0,1.0));
    std::vector<fireball*> bolas_de_fogo;
    // tesouro que o drag�o guarda (vai dar o poder de ataque � dist�ncia e permitir matar o chefe)
    fireball *treasure_fireball = new fireball(velocidade_objeto_inanimado, glm::vec4(pedestal_do_dragao.getPosition().x,pedestal_do_dragao.getPosition().y+5.0f,pedestal_do_dragao.getPosition().z,1.0f), glm::vec3(1.5f,1.5f,1.5f), glm::vec4(0.0f,0.0f,0.0f,0.0f));

    //Kraken
    STANDARD_KRAKEN_ATTRIBUTES atributos_kraken;
    kraken chefe(atributos_kraken, glm::vec4(0.0f, 0.0f, 30.0f, 1.0f), glm::vec4(0.0f, 0.0f, 30.0f, 0.0f), glm::vec3(5.0,5.0,5.0));

    // objeto que calcula a curva de bezier para o movimento do drag�o
    double t_anterior = 0.0f;
    double t_atual;
    const double duracao_bezier = 5.0;
    bool esta_na_direcao_bezier_original = true;
    bezierBuilder bezier;

    //pontos da curva bezier c�bica do drag�o
    glm::vec4 p0(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 p1(2.0f, 5.0f, 0.0f, 1.0f);
    glm::vec4 p2(5.0f, -5.0f, 0.0f, 1.0f);
    glm::vec4 p3(10.0f, 0.0f, 0.0f, 1.0f);

    // Ficamos em um loop infinito, renderizando, at� que o usu�rio feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as opera��es de renderiza��o

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor �
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto �:
        // Vermelho, Verde, Azul, Alpha (valor de transpar�ncia).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Ilumina��o.
        //
        //           R     G     B     A
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e tamb�m resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de v�rtice e fragmentos).
        glUseProgram(shaders.g_GpuProgramID);

        // Computamos o sentido para onde a c�mera est� olhando utilizando coordenadas esf�ricas.  As
        // vari�veis g_CameraDistance, g_CameraPhi, e g_CameraTheta s�o
        // controladas pelo mouse do usu�rio. Veja as fun��es CursorPosCallback()
        // e ScrollCallback().
        // C�mera n�o pode se mover enquanto o jogador estiver atacando corpo-a-corpo
        if(!is_attacking){
            r = g_CameraDistance;
            y = r*sin(g_CameraPhi);
            z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
            x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
        }

        // Defini��o da matriz "View", que determina o sistema de coordenadas da c�mera
        glm::mat4 view;

        // c�lculo para determinar a atualiza��o de desenhos para anima��es
        tempo_atual = glfwGetTime();
        deltat = tempo_atual - tempo_anterior;
        tempo_anterior = tempo_atual;

        // "F" para toggle da c�mera
        // se escolhemos a c�mera em primeira pessoa
        if(g_CameraFirstPerson){
            // Seta a posi��o a posi��o da c�mera no mesmo local da cabe�a do modelo do jogador (no come�o, em caso de que a c�mera seja trocada de terceira para primeira pessoa)
            cam.setCameraPosition(jogador.getPlayerHeadPosition());

            jogador.setViewDirection(glm::vec4(x,y,z,0.0f)); // Vetor "view" do jogador (para onde o modelo estar� olhando)

            //Determina��o do vetor view (free camera) da c�mera, que determina para onde ela est� olhando
            cam.setViewVector(jogador.getViewDirection()); // olha para a mesma dire��o que o modelo
        // se escolhemos a c�mera em terceira pessoa
        } else {
            // Seta a posi��o da c�mera um pouco atr�s da dire��o onde o jogador est� olhando (no come�o, em caso de que a c�mera seja trocada de primeira para terceira pessoa)
            cam.setCameraPosition(glm::vec4(jogador.getPlayerHeadPosition().x-(x*g_CameraDistance),jogador.getPlayerHeadPosition().y-(y*g_CameraDistance),jogador.getPlayerHeadPosition().z-(z*g_CameraDistance),1.0f));

            jogador.setViewDirection(glm::vec4(x,0.0f,z,0.0f)); // Vetor "view" do jogador (para onde o modelo estar� olhando)

            //Determina��o do vetor view da c�mera, que determina para onde ela est� olhando (look-at-camera)
            cam.setViewVector(jogador.getPlayerHeadPosition() - cam.getCameraPosition());
        }

        cam.setCameraFowardMoveVector(glm::vec4(x,0.0f,z,0.0f)); // vetor que determina a dire��o para onde a c�mera pode se mexer quando andamos para frente/tr�s. Impede a c�mera de "voar" para longe

        // Determina��o dos vetores w e u, que definem o sistema de coordenadas da c�mera
        jogador.setPlayerWVector(cam.getCameraFowardMoveVector()/norm(cam.getCameraFowardMoveVector())); // Vetor "w" aponta para "tr�s" da c�mera
        jogador.setPlayerUVector(crossproduct(cam.getUpVector(), jogador.getPlayerWVector())/norm(crossproduct(cam.getUpVector(), jogador.getPlayerWVector()))); // Vetor "u" aponta para "direita" da c�mera

        // Normaliza��o dos vetores w e u
        jogador.setPlayerWVector(jogador.getPlayerWVector()/norm(jogador.getPlayerWVector()));
        jogador.setPlayerUVector(jogador.getPlayerUVector()/norm(jogador.getPlayerUVector()));

        // Atualiza��o da posi��o do personagem control�vel. Jogador n�o pode se mexer se estiver atacando corpo-a-corpo
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
        // segue a l�gica de como o ataque deve funcionar, dependendo se o jogador desbloqueou ataques � dist�ncia ou n�o
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
        // movimento da bola de fogo se o jogador tiver desbloqueado ataques � dist�ncia. Continua at� o jogador realizar um novo ataque (ou at� a bola atingir um alvo)
        if(bola_jogador != nullptr){
            bola_jogador->setPosition(bola_jogador->getPosition() += scalarMultiplication(bola_jogador->getViewDirection(),bola_jogador->getSpeed()*deltat));
        }

        // Atualiza��o da posi��o da c�mera de acordo com a vis�o (primeira ou terceira pessoa)
        if(g_CameraFirstPerson){
            cam.setCameraPosition(jogador.getPlayerHeadPosition());
        } else{
            cam.setCameraPosition(glm::vec4(jogador.getPlayerHeadPosition().x-(x*g_CameraDistance),jogador.getPlayerHeadPosition().y-(y*g_CameraDistance),jogador.getPlayerHeadPosition().z-(z*g_CameraDistance),1.0f));
        }

        view = Matrix_Camera_View(cam.getCameraPosition(), cam.getViewVector(), cam.getUpVector());

        // atualiza a posi��o das lulas
        // elas andam na dire��o do jogador se ele estiver pr�ximo delas
        // e voltam para a posi��o inicial se ele se afastar
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

        // Atualiza��o da posi��o do drag�o
             // tempo_atual % duracao_bezier
        t_atual = fmod(tempo_atual, duracao_bezier) / duracao_bezier;
        // se chegar ao fim do caminho da curva bezier, inverte a dire��o
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

        // Se o jogador estiver a uma certa dist�ncia do drag�o, o drag�o o atacar� com bolas de fogo
        if(norm(jogador.getPosition() - dragao.getPosition()) < dragao.getDetectionRadius()){
            if(dragao.attackIsValid(tempo_atual)){
                fireball *nova_bola = new fireball(velocidade_bola_de_fogo_dinamica, dragao.getPosition(), glm::vec3(1.0,1.0,1.0), dragao.getViewDirection());
                bolas_de_fogo.push_back(nova_bola);
            }
        }

        // movimento das bolas de fogo do drag�o
        for(auto bola_de_fogo : bolas_de_fogo){
            bola_de_fogo->setPosition(bola_de_fogo->getPosition() += scalarMultiplication(bola_de_fogo->getViewDirection(),deltat));
        }

        // Agora computamos a matriz de Proje��o.
        glm::mat4 projection;

        // Note que, no sistema de coordenadas da c�mera, os planos near e far
        // est�o no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
        float nearplane = -0.1f;  // Posi��o do "near plane"
        float farplane  = -150.0f; // Posi��o do "far plane"

        // Proje��o Perspectiva.
        // Para defini��o do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);

        glm::mat4 model = Matrix_Identity(); // Transforma��o identidade de modelagem

        // Enviamos as matrizes "view" e "projection" para a placa de v�deo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas s�o
        // efetivamente aplicadas em todos os pontos.
        glUniformMatrix4fv(shaders.g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(shaders.g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

        // defines para ajudar na associa��o dos modelos
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
        // conceito auxiliado por chatgpt. Todos os objetos estavam com a primeira textura setada em TextureImage0 at� realizarmos esta mudan�a
        // associando os samplers2D �s texturas
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
        // Precisamos pedir para que o opengl fa�a o culling das faces externas da esfera
        glCullFace(GL_FRONT);
        model = Matrix_Translate(jogador.getPosition().x,jogador.getPosition().y,jogador.getPosition().z) // Skybox fica relativa ao jogador
              * Matrix_Scale(100.0f, 100.0f, 100.0f);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, SKYBOX);
        DrawVirtualObject("the_sphere", g_VirtualScene);
        // Fazemos o culling das faces internas dos outros objetos
        glCullFace(GL_BACK);

        for(squid& instancia_de_lula : instancias_de_lulas){
            // desenhamos a "cabe�a" das lulas
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

        // Desenhamos o personagem jog�vel
        model = Matrix_Translate(jogador.getPosition().x,jogador.getPosition().y,jogador.getPosition().z)
              * Matrix_Scale(jogador.getScalingFactor().x,jogador.getScalingFactor().y,jogador.getScalingFactor().z)
              * Matrix_Rotate_Y(atan2(jogador.getViewDirection().x, jogador.getViewDirection().z));
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, CHEVALIER);
        DrawVirtualObject("chevalier", g_VirtualScene);

        // Desenhamos o Drag�o
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

        // Desenhamos as bolas de fogo do drag�o
        for(auto bola_de_fogo : bolas_de_fogo){
            model = Matrix_Translate(bola_de_fogo->getPosition().x,bola_de_fogo->getPosition().y,bola_de_fogo->getPosition().z);
            glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(shaders.g_object_id_uniform, FIREBALL);
            DrawVirtualObject("sphere", g_VirtualScene);
        }

        // Desenhamos o ch�o
        model = Matrix_Translate(chao.getPosition().x,chao.getPosition().y,chao.getPosition().z)
              * Matrix_Scale(chao.getScalingFactor().x,chao.getScalingFactor().y,chao.getScalingFactor().z);
        glUniformMatrix4fv(shaders.g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(shaders.g_object_id_uniform, GROUND);
        DrawVirtualObject("groundAlow", g_VirtualScene);

        // Desenhamos o Kraken:
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            // desenhamos a "cabe�a" do kraken
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

        // Desenhamos a bola de fogo que fica em cima do pedestal, se ela n�o foi coletada
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

        // Imprimimos na tela informa��o sobre o n�mero de quadros renderizados
        // por segundo (frames per second).
        if(g_ShowInfoText){
            TextRendering_ShowFramesPerSecond(window, g_ShowInfoText);
        }

        // O framebuffer onde OpenGL executa as opera��es de renderiza��o n�o
        // � o mesmo que est� sendo mostrado para o usu�rio, caso contr�rio
        // seria poss�vel ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usu�rio
        // tudo que foi renderizado pelas fun��es acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma intera��o do
        // usu�rio (teclado, mouse, ...). Caso positivo, as fun��es de callback
        // definidas anteriormente usando glfwSet*Callback() ser�o chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Definimos o callback para impress�o de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Defini��o da fun��o que ser� chamada sempre que a janela do sistema
// operacional for redimensionada, por consequ�ncia alterando o tamanho do
// "framebuffer" (regi�o de mem�ria onde s�o armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda regi�o do framebuffer. A
    // fun��o "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa � a opera��o de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos tamb�m a raz�o que define a propor��o da janela (largura /
    // altura), a qual ser� utilizada na defini��o das matrizes de proje��o,
    // tal que n�o ocorra distor��es durante o processo de "Screen Mapping"
    // acima, quando NDC � mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float � necess�rio pois n�meros inteiros s�o arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Fun��o callback chamada sempre que o usu�rio aperta algum dos bot�es do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usu�rio pressionou o bot�o esquerdo do mouse setamos a vari�vel
        // g_LeftMouseButtonPressed como true, para saber que o usu�rio est�
        // com o bot�o esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usu�rio soltar o bot�o esquerdo do mouse, atualizamos a
        // vari�vel abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
}

// Fun��o callback chamada sempre que o usu�rio movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Abaixo executamos o seguinte: computamos quanto que o mouse se movimento desde o �ltimo
    // instante de tempo, e usamos esta movimenta��o para atualizar os
    // par�metros que definem a posi��o da c�mera dentro da cena virtual.
    // Assim, temos que o usu�rio consegue controlar a c�mera.

    // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
    if(!is_attacking){
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;


        // Atualizamos par�metros da c�mera com os deslocamentos
        g_CameraTheta -= 0.01f*dx;
        g_CameraPhi   -= 0.01f*dy;

        // Em coordenadas esf�ricas, o �ngulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2;
        float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        // Atualizamos as vari�veis globais para armazenar a posi��o atual do
        // cursor como sendo a �ltima posi��o conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Fun��o callback chamada sempre que o usu�rio movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Atualizamos a dist�ncia da c�mera para a origem utilizando a
    // movimenta��o da "rodinha", simulando um ZOOM.
    // Mas apenas quando estivermos em terceira pessoa
    if(!g_CameraFirstPerson){
        g_CameraDistance -= 0.1f*yoffset;
    }

    // Uma c�mera look-at nunca pode estar exatamente "em cima" do ponto para
    // onde ela est� olhando, pois isto gera problemas de divis�o por zero na
    // defini��o do sistema de coordenadas da c�mera. Isto �, a vari�vel abaixo
    // nunca pode ser zero. Vers�es anteriores deste c�digo possu�am este bug,
    // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (g_CameraDistance < verysmallnumber)
        g_CameraDistance = verysmallnumber;
}

// Defini��o da fun��o que ser� chamada sempre que o usu�rio pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usu�rio pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Se o usu�rio apertar a tecla F, fazemos um "toggle" entre primeira e terceira pessoa.
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        g_CameraFirstPerson = !g_CameraFirstPerson;
    }

    // Se o usu�rio apertar a tecla W, movemos a posi��o da c�mera para "frente"
    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS){
            teclas.g_wKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_wKeyPressed = false;
        }
    }

    // Se o usu�rio apertar a tecla S, movemos a posi��o da c�mera para "tr�s"
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS){
            teclas.g_sKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_sKeyPressed = false;
        }
    }

    // Se o usu�rio apertar a tecla D, movemos a posi��o da c�mera para "direita"
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS){
            teclas.g_dKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_dKeyPressed = false;
        }
    }

    // Se o usu�rio apertar a tecla A, movemos a posi��o da c�mera para "esquerda"
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS){
            teclas.g_aKeyPressed = true;
        }
        else if (action == GLFW_RELEASE){
            teclas.g_aKeyPressed = false;
        }
    }

    // Se o usu�rio apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :



