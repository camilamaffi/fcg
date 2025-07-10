# fcg
**Relatório do Trabalho Final de Fundamentos de Computação Gráfica**

![image](https://github.com/user-attachments/assets/bd3a2139-f9f8-4be6-861b-52338a842267)
![imagem2_fcg](https://github.com/user-attachments/assets/2d711e0f-c9bb-4334-983c-85d1a38a7cbb)

**Todas as Contribuições do Projeto foram feitas por Bernardo Fellini Oliveira e incluem:**
- Implementação de todos os modelos de objetos
- Implementação de todas as texturas nos componentes do jogo
- Implementação de todas as colisões e física do jogo
- Implementação de toda a iluminação do jogo
- Implementação da câmera e de todos os controles por parte do usuário
- Implementação de toda a lógica de movimentação dos objetos do jogo

**Eu fiz uso do chatGPT para a escrita de algumas funções:**
- em main.cpp: me ajudou a realizar o uso de glGetUniformLocation() e glUniform1i() para associar corretamente as texturas aos modelos
- em collisions.cpp: me ajudou a escrever os testes de colisão caixa-esfera entre as boundingBoxes/Spheres dos modelos
- em bezierBuilder.cpp: me ajudou a escrever a função que calcula uma curva de Bézier de grau 3 dados 4 pontos e o fator t [0.0,1.0]
Fora isso, o chatGPT foi mais útil na hora de realizar debugging, conseguindo facilmente encontrar e explicar os erros sobre os quais eu ainda não tinha conhecimento teórico o suficiente para resolver, 
especialmente quando os erros envolviam desconhecimento sobre o comportamento dos processos de compilação e linkagem da linguagem c++

**Aplicação dos requisitos requeridos:**
- Aplicação possibilita interação em tempo real
- Aplicação possui objetivo (matar Kraken) e lógica de controle não-trivial (requer interação com o tesouro para permitir matar o Kraken)
- Não há uso de funções de bibliotecas pré-existentes para transformações geométricas, projeções e especificação do sistema de coordenadas da caâmera
- Aplicação permite interação com o usuário usando mouse e teclado (mover o personagem com w,a,s,d; mover a câmera com o mouse; atacar com o botão esquerdo do mouse)
- Aplicação possui objetos de malahs poligonais complexas (objetos rocha e caverna são relativamente complexos)
- Aplicação possui transformações geométricas de objetos virtuais interativas com teclado (movimentação do modelo do personagem) e mouse (lançamento de bolas de fogo com ataque)
- Aplicação possui pelo menos 1 objeto virtual copiado em 3 instâncias (inimigo "lula" possui 3 instâncias aplicadas a um mesmo objeto)
- Aplicação possui 3 testes de intersecção distintos entre objetos virtuais (exemplos: cubo-cubo -> kraken com chão / esfera-esfera -> personagem jogável e inimigos lula / cubo-esfera -> bola de fogo do personagem e inimigos lula)
- Aplicação possui objetos com iluminação difusa (Lambert) e Blinn-Phong (todos os objetos)
- Aplicação possui objetos com modelos de interpolação para iluminação Gouraud (chão) e Phong (todos os outros)
- Aplicação possui todos os objetos com texturas mapeadas
- Aplicação possui curva de Bézier (movimentação do inimigo dragão)
- Aplicação possui todas as animações de movimento baseadas no tempo Δt 

**Como Executar:**
1) Clone este repositório (git clone https://github.com/camilamaffi/fcg.git)
2) Tenha alguma IDE que tenha suporte para aplicações em C/C++ (de preferência CODE::BLOCKS, que já vem com compilador embutido) e algum compilador GCC, se já não vier embutido
3) Abra o project file (Trabalho Final FCG) com a IDE e compile-o
4) Execute o executável criado na pasta bin

**Como jogar:**
W, A, S, D para mover o personagem
Movimente o mouse para mover a câmera
Botão esquerdo do mouse para atacar
F para mudar entre primeira e terceira pessoa
H para mostrar/esconder o FPS

O objetivo é ir até o tesouro (a bola de fogo encima do podestal) guardado pelo dragão e 3 pequenas lulas para ganhar o poder de lançar suas bolas de fogo à distância.
Isso lhe permitirá matar o Kraken que está no início do mapa
