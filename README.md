# visualizador_3d - Trabalho de Oficina de Computação (Prof Carlos Maziero)

Este projeto consiste em construir um programa para ler uma descrição de objeto em 3 dimensões de um arquivo em disco e apresentar na tela a visualização desse objeto, considerando uma projeção em perspectiva.

![teapot](teapot.png?raw=true "Teapot")

Os requisitos do projeto são:
- Ler a descrição de objeto 3D de um arquivo em disco. O objeto está descrito no formato Wavefront OBJ, usado em ferramentas de modelagem/visualização 3D como Blender, 3Dstudio e Rhino.
- Calcular a projeção em perspectiva dos vértices e arestas do objeto.
- Mostrar em uma janela gráfica a projeção obtida com representação wireframe, usando uma biblioteca gráfica.
- Usar as teclas de setas left/right e up/down para mudar a posição do observador nos eixos X e Y, respectivamente, recalculando a projeção a cada mudança de posição.
- Usar a tecla “ESC” ou “q” para sair do programa.

## Formato Wavefront OBJ
O formato de dados Wavefront OBJ é considerado um “formato universal” para a representação de objetos em 3 dimensões, sendo reconhecido pela maioria dos softwares de modelagem/visualização em 3D.

Nesse tipo de arquivo, os dados do objeto são representados em formato ASCII. Os dados com representação mais frequente são os vértices e as faces:
- um vértice é um ponto no espaço, com coordenadas (x, y, z) em ponto flutuante.
- uma face é uma sequência de 3 ou mais vértices que define uma superfície.

Um exemplo simples de objeto 3D no formato OBJ:

**cubo.obj**
```
# OBJ - Wavefront object file
# Esta linha contém um comentário
 
# v <x> <y> <z>  - Define um vértice do objeto (podem ser int ou float)
v 16 32 16
v 16 32 -16
v 16 0 16
v 16 0 -16
v -16 32 16
v -16 32 -16
v -16 0 16
v -16 0 -16
 
# definição das faces (v1 v2 v3 v4 ...)
f 1 3 4 2
f 6 8 7 5
f 2 6 5 1
f 3 7 8 4
f 1 5 7 3
f 4 8 6 2
```

Além dos vértices e faces, o formato OBJ também permite definir linhas, pontos, normais, texturas e outros elementos importantes em um modelo 3D.


## Projeção em perspectiva
A técnica de projeção 3D consiste em transformar uma representação 3D em 2D, ou seja, com todos os seus pontos em um mesmo plano, possibilitando então sua visualização na tela do computador.

Existem vários tipos de projeção, como a projeção ortográfica e a projeção em perspectiva. Na projeção em perspectiva, a posição do observador é considerada nos cálculos da transformação 3D-2D.

Os cálculos necessários à projeção podem ser complexos e demorados, mas algumas simplificações podem ser realizadas. Este projeto usa uma simplificação chamada perspectiva fraca (weak perspective), descrita a seguir.

### Perspectiva fraca
Considerando que as coordenadas da câmera são [xc yc zc] e que a câmera está olhando para a origem [0 0 0], a conversão das coordenadas 3D de cada vértice v = [xv yv zv] em sua projeção 2D p = [xp yp] no plano z = 0 pode ser calculada desta forma:

- xp = xc + zc × ( (xv - xc) ÷ (zv + zc) )
- yp = yc + zc × ( (yv - yc) ÷ (zv + zc) )
Assim é obtida uma coleção de pontos [xp yp] que representa os vértices do objeto projetados no plano z=0.

***Conversão para coordenadas de tela***
Para que os pontos obtidos na etapa anterior possam ser plotados na tela, eles devem primeiro ser ajustados para o intervalo [(0,0) ... (width, height)], onde width e height são as dimensões da janela de visualização, em pixels.

Esta etapa transforma o o conjunto de pontos projetados [xp yp] em um conjunto de pontos de desenho [xd yd], que podem ser usados para plotar as arestas dos objetos na tela.

Passo 1: calcular mínimos, máximos, centros e diferenças das coordenadas em X e Y
```
xmin = min (xp)
xmax = max (xp)
xcen = (xmax + xmin) / 2
xdif = xmax - xmin
ymin = min (yp)
ymax = max (yp)
ycen = (ymax + ymin) / 2
ydif = ymax - ymin
```

Passo 2: calcular fator de escala para o desenho na tela
```
W: largura da janela de desenho (em pixels)
H: altura da janela de desenho (em pixels)
escx = W / xdif
escy = H / ydif
escala = min (escx, escy)
```

Passo 3: centrar pontos da projeção em [0 0]
```
∀ (x y)
x'p = xp - xcen
y'p = yp - ycen
```

Passo 4: ajustar escala dos pontos da projeção para a tela
```
∀ (x y)
x“p = x'p * escala
y”p = y'p * escala
```

Passo 5: ajustar pontos do desenho em relação ao centro da tela
```
∀ (x y)
xd = x“p + W / 2
yd = y”p + H / 2
```

Os passos 3 a 5 podem ser condensados em um único passo:
```
∀ (x y)
xd = ( (xp - xcen) * escala) + W / 2
yd = ( (yp - ycen) * escala) + H / 2
```

Com isso é obtido um conjunto de pontos no intervalo [(0, 0) .. (width, height)] que pode ser usado para plotar na janela gráfica as arestas que definem o objeto 3D.


### Atividade
Para apresentar a visualização de um objeto 3D, o programa deve:

- Ler o conjunto de vértices e faces do arquivo de entrada
- Modelos grandes podem conter milhões de vértices, portanto o uso de alocação dinâmica de memória é obrigatório.
- Para tratar os dados do arquivo OBJ, sugere-se usar a função strtok.
- Para uma dada posição da câmera, calcular as projeções 2D dos vértices 3D.
- Converter os pontos projetados para coordenadas de tela; sugere-se usar uma janela de 800×600 pixels.
- Desenhar as faces na tela, produzindo uma representação wireframe do objeto. Para desenhar uma face, desenhe separadamente as arestas que a compõem. Por exemplo, para desenhar a face (1 2 3 4), desenhe as arestas (1 2), (2 3), (3 4) e (4 1).
- Ler as teclas de setas (← → ↑ ↓), ajustar as coordenadas da câmera, recalcular a projeção e desenhá-la novamente.
- Tecla ESC (ou fechar a janela) para sair do programa.
- Formas de chamada do executável (ambas devem ser implementadas):

### usando argc/argv
```
wireframe arquivo.obj
``````

### usando stdin  
```
wireframe < arquivo.obj
```
ou
```
cat arquivo.obj | wireframe
```

A implementação deve atender os seguintes requisitos:

- Usar as funções da biblioteca gráfica indicada pelo professor
- Usar struct para representar os elementos do modelo (vértices, faces, arestas)
- Alocar memória para os elementos do modelo de forma dinâmica (malloc/free)
- Funcionar para todos os exemplos disponíveis nesta página
- Estrutura sugerida de arquivos do código-fonte:
- wireframe.c: arquivo principal
- datatypes.h: tipos de dados usados no programa
- objread.c/h: funções de leitura do arquivo OBJ
- perspect.c/h: funções de cálculo de perspectiva
- graphics.c/h: funções de apresentação gráfica
- Usar Makefile
- cláusulas all (default), clean e purge
- usar flag de compilação -Wall
- separar as fases de compilação e de ligação
- usar regras implícitas
