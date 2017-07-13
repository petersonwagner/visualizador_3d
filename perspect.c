#include <stdio.h>
#include <stdlib.h>
#include "datastruct.h"

void weak_perspective (coord3_t camera, vlist_t *vertices, coord2_t **draw_points)
{
	int i;
	float scale;
	coord2_t max, min, dif, center;
	coord2_t *proj;

	//pressupondo que o minimo e maximo seja
	//o primeiro vertice apenas para inciialização
	min.x = max.x = vertices->array[0].x;
	min.y = max.y = vertices->array[0].y;

	//alocação do conjunto temporario de pontos projetados no plano z=0
	proj = (coord2_t*) malloc (sizeof(coord2_t) * vertices->size);

	for (i = 0; i < vertices->size; ++i)
	{
		//projecao dos vertices no plano z=0
		proj[i].x = camera.x + camera.z * ((vertices->array[i].x - camera.x) / (vertices->array[i].z + camera.z));
		proj[i].y = camera.y + camera.z * ((vertices->array[i].y - camera.y) / (vertices->array[i].z + camera.z));

		//busca dos minimos e maximos dos pontos projetados
		if (proj[i].x > max.x)
			max.x = proj[i].x;
		if (proj[i].x < min.x)
			min.x = proj[i].x;

		if (proj[i].y > max.y)
			max.y = proj[i].y;
		if (proj[i].y < min.y)
			min.y = proj[i].y;
	}

	//calculo da diferença
	dif.x = max.x - min.x;
	dif.y = max.y - min.y;

	//calculo do centro
	center.x = (max.x + min.x) / 2;
	center.y = (max.y + min.y) / 2;

	//calculo do fator de escala
	//aqui, diminuo 1 da altura e da largura da tela porque
	//o intervalo da tela é [(0,0)..(largura-1,altura-1)]
	if ((SCREEN_WIDTH / dif.x) < (SCREEN_HEIGHT / dif.y))
		scale = (SCREEN_WIDTH  - 1) / dif.x;   
	else
		scale = (SCREEN_HEIGHT - 1) / dif.y;

	//calculo do conjunto de pontos projetados na tela
	//no intervalo [(0,0)..(width,height)]
	//com a inversao vertical da tela
	for (i = 0; i < vertices->size; ++i)
	{
		(*draw_points)[i].x = (SCREEN_WIDTH  / 2) + ((proj[i].x - center.x) * scale);
		(*draw_points)[i].y = (SCREEN_HEIGHT / 2) - ((proj[i].y - center.y) * scale);
	}

	//free no conjunto temporario de pontos projetados no plano z=0
	free (proj);
}