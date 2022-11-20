# Universidade Federal do ABC

## Autor

* **Nome: Leandro de Souza Mattos**
* **RA: 11049914**

# Descrição

**Jogo baseado no Pacman**

O projeto consiste em uma representação 3D de um mundo com fanstama e neblina, o foco do personagem pactan é se alimentar para ganhar pontos. O pactan é controlado a partir do teclado (space) e a camera através do mouse, e para se aproximar ou se afastar do pactan, usa o scroll do mouse.

# Implementação

O jogo se baseia em 5 classes que representam a janela do jogo, o **pactan**, **fantasma**, **food** e **câmera**. Elas estão definidas abaixo, acompanhadas de breves descrições das funções e atributos relevantes.

## Camera

```cpp
* void Camera::computeViewMatrix(); //A cena é observada a partir de uma câmera que segue os movimentos do pactan
```

* rotaciona a camera no eixo X, realizando o  pitch
* rotaciona a camera no eixo y, realizando o yaw
* deslocamento da camera

```cpp
  void Camera::update(); //atualiza a posição da câmera em relação ao Pactan
  
  void Camera::updatePosition(); //movimento do mouse atual
  
  void Camera::pitch(); //movimento de pitch, com base no movimento relativo do mouse

  void Camera::angulo();  //gira a camera em torno do pactan;

```

## Fantasma

```cpp
* fantasma(); //inicia a textura do fantasma fixados no ambiente do jogo, essas texturas são carregadas da pasta assets, 
// É definida a distancia entre os fantasmas randomicamente.
```
## Food  
 
```cpp
food();

void Food::initializeGL(); //gera os cubos de comida que estão na pasta assets

void Food::update(); //rotaciona a comida e verifica se colidiu com o player

void Food::computeModelMatrix(); //definição da comida no ambiente

void Food::paintGL(); //se a comida foi capturada pelo player, nao sera mais renderizada

FoodUnidade* Food::criaFoodUnidade(); //ao gerar uma contabilização de food ingeridos que servirá como score do player
```

## Pactan

O pactan pode se mover por todos os lados, pode se alimentar para aumentar a sua pontuação e deve fugir de colidir com o fantasma.
A medida que vai se alimentando, os cubos vão sumindo e para assim aumentar o score.

```cpp
pactan();

void Pactan::initializeGL(); //carrega o objeto pactan que está na pasta assets

void Pactan::paintGL(); //movimento da câmera em relação ao Pactan no ambiente

void Pactan::pular(); //com a tecla space o Pactan pula e torna ao chão de acordo com a gravidade

void Pactan::update(); //atualiza velocidade de rotação, posição, distancia e gravidade
```

## Wall

```cpp
wall(); //carrega as texturas referente a parede do ambiente

void Wall::paintGL(); //como a câmera fixa o chão do ambiente em relação ao pactan
```
