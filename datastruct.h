#ifndef __DATASTRUCT__
#define __DATASTRUCT__

#define true  1
#define false 0

//altura e largura da tela constantes,
//ja que nao é prevista alteração nestes valores
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

typedef struct coord3_t
{
	float x;
	float y;
	float z;
} coord3_t;

typedef struct coord2_t
{
	float x;
	float y;
} coord2_t;

typedef struct vlist_t
{
	int size;
	int last_index;
	coord3_t *array;
} vlist_t;

typedef struct line_t
{
	int v1;
	int v2;
} line_t;

typedef struct linelist_t
{
	line_t *array;
	int size;
} linelist_t;

#endif