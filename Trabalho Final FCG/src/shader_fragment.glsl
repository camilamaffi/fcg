#version 330 core
// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;
in vec3 cor_v;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Identificador que define qual objeto está sendo desenhado no momento
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
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;
uniform sampler2D TextureImage9;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    // Fazer com que v == l no nosso caso conserta os erros estranhos de cor que estavam acontecendo em versões antigas
    // Mantemos a distinção entre v e l por razões semânticas
    vec4 l = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = l + 2*n*(dot(n,l)); // PREENCHA AQUI o vetor de reflexão especular ideal

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    if ( object_id == KRAKEN_HEAD )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage0, vec2(U,V)).rgb;
        Ks = texture(TextureImage0, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == KRAKEN_BODY )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage0, vec2(U,V)).rgb;
        Ks = texture(TextureImage0, vec2(U,V)).rgb;

        // Coordenadas
        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == KRAKEN_EYE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage1, vec2(U,V)).rgb;
        Ks = texture(TextureImage1, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == CHEVALIER ){
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage2, vec2(U,V)).rgb;
        Ks = texture(TextureImage2, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == SKYBOX ){

        // Coordenadas de textura em Sphere Projection Mapping
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        float ro = 1.0f; // raio = 1
        vec4 p_circulo = bbox_center + (ro * ((position_model - bbox_center)/(sqrt( v.x*v.x + v.y*v.y + v.z*v.z ))));
        vec4 v_circulo = p_circulo - bbox_center;

        float theta = atan(v_circulo.x,v_circulo.z);
        float phi = asin(v_circulo.y/ro);

        U = (theta + M_PI)/(2.0f*M_PI);
        V = (phi + M_PI_2)/M_PI;

        Kd = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if( object_id == DRAGON ){
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage4, vec2(U,V)).rgb;
        Ks = texture(TextureImage4, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if( object_id == FIREBALL ){
        // Coordenadas de textura em Sphere Projection Mapping
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        float ro = 1.0f; // raio = 1
        vec4 p_circulo = bbox_center + (ro * ((position_model - bbox_center)/(sqrt( v.x*v.x + v.y*v.y + v.z*v.z ))));
        vec4 v_circulo = p_circulo - bbox_center;

        float theta = atan(v_circulo.x,v_circulo.z);
        float phi = asin(v_circulo.y/ro);

        U = (theta + M_PI)/(2.0f*M_PI);
        V = (phi + M_PI_2)/M_PI;

        Kd = texture(TextureImage5, vec2(U,V)).rgb;
        Ks = texture(TextureImage5, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == GROUND ){
        //Coordenadas de textura em Cube Projection Mapping.
        /* Não estão sendo usadas. estão aqui só para caso precisarmos usá-las em um outro modelo futuramente
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = (position_model.x-minx)/(maxx-minx);
        V = (position_model.y-miny)/(maxy-miny);*/

        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage6, vec2(U,V)).rgb;
        Ks = texture(TextureImage6, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

       // Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == ROCK ){
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage7, vec2(U,V)).rgb;
        Ks = texture(TextureImage7, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == PEDESTAL ){
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage8, vec2(U,V)).rgb;
        Ks = texture(TextureImage8, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == CAVE ){
        U = texcoords.x;
        V = texcoords.y;

        Kd = texture(TextureImage9, vec2(U,V)).rgb;
        Ks = texture(TextureImage9, vec2(U,V)).rgb;

        U = atan(normal.z, normal.x)/(2.0f*M_PI);
        V = asin(normal.y)/M_PI;

        Ka = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else // Objeto desconhecido = preto
    {
        Kd = vec3(0.0,0.0,0.0);
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.0,0.0,0.0);
        q = 1.0f;
    }

    q = 1.0f;

    // Espectro da fonte de iluminação
    vec3 I = vec3(0.6,0.6,0.6);

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2);

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd*I*max(0,dot(n,l));

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));

    // Termo ambiente
    vec3 ambient_term = Ka*Ia;

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks*I*pow(max(0,dot(r,v)),q);

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
    if ( object_id == GROUND){
        color.rgb = cor_v;
    }
    else if ( object_id != SKYBOX){
        color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;
    }
    else{
        color.rgb = Kd; // Skybox não usa cor ambiente ou especular. É definida apenas pela textura
    }

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}
