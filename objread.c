#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commline.h"
#include "datastruct.h"

//funcao para ler uma linha inteira
//ate encontrar uma quebra de linha ou fim de arquivo,
//sem armazenar nada,
void get_line (FILE *stream)
{
	char c;

	do
	{
		c = fgetc (stream);
	} while (c != EOF && c != '\n');
}

//funcao para fazer a leitura de uma linha de vertices
void read_vertice(FILE *stream, vlist_t *vertices, int *last_index)
{
	#define array  vertices->array
	#define size   vertices->size

	//se o vetor de vertices estiver vazio
	if (array == NULL)
	{
		array = (coord3_t*) malloc (sizeof(coord3_t) * 4);
		size = 4;
		*last_index = 0;
	}
	//se o tamanho do vetor ja nao for mais suficiente,
	//dobrar o seu tamanho com realloc
	else if (*last_index > size - 1)
	{
		size = size * 2;
		array = (coord3_t*) realloc (array, sizeof(coord3_t) * size);
	}

	fscanf(stream, "%f", &array[*last_index].x);
	fscanf(stream, "%f", &array[*last_index].y);
	fscanf(stream, "%f", &array[*last_index].z);

	//aumenta um no indicador do ultimo indice escrito do vetor
	(*last_index)++;

	#undef array
	#undef size
}

//funcao que trata as linhas de faces com valores adicionais
//
//ex de entrada: "1111/2222/3333 4444/5555/6666 7777/8888/9999"
//tratamento:    "1111           4444           7777          "
//
//dessa forma, é possível fazer o parsing mais facilmente
//(e também de uma maneira um pouco mais fabulosa que usando strtok)
//
//essa funcao retorna o numero de valores a ser lido por alguma funcao posterior
int get_string (char *string, FILE *stream)
{
	int size = 0;
	unsigned int i;
	char c;

	//le a string ate encontrar uma quebra de linha
	for (i = 0, c = fgetc(stream); c != '\n'; c = fgetc(stream), i++)
		string[i] = c;
	string[i] = '\0';

	//conta numero de elementos
	for (i = 0; i < strlen(string); ++i)
	{
		//se encontrar um numero
		if (string[i] >= '0' && string[i] <= '9')
		{
			//ler todos os numeros ate encontrar um espaço, '/' ou fim da string
			do
			{
				i++;
			} while (string[i] != ' ' && string[i] != '/' && i < strlen(string));

			size++;
		}

		//se encontrar uma barra
		if (string[i] == '/')
		{
			//substituir todos os caracteres por espaço ate encontrar um espaço ou fim da string
			do
			{
				string[i] = ' ';
				i++;
			} while (string[i] != ' ' && i < strlen(string));
		}
	}

	return size;
}

//funcao que insere uma linha no vetor de linhas
void insert_line (linelist_t *lines, line_t new_line, int *last_index)
{
	#define array lines->array

	array[*last_index] = new_line;
	*last_index = *last_index + 1;

	#undef array
}

//funcao para fazer a leitura de uma linha de faces
void read_face(FILE *stream, linelist_t *lines, int *last_index)
{
	FILE *str_stream = NULL;
	char string[200];
	int size, i, first;
	line_t new_line;

	size = get_string (string, stream);

	//se o vetor de linhas estiver vazio
	if (lines->array == NULL)
	{
		lines->array = (line_t*) malloc (sizeof(line_t) * size);
		lines->size = size;
	}
	//se o tamanho do vetor ja nao for mais suficiente,
	//dobrar o seu tamanho com realloc
	else if (lines->size <= (*last_index) + size)
	{
		lines->size = lines->size * 2;
		lines->array = (line_t*) realloc (lines->array, sizeof(line_t) * lines->size);
	}

	//joga a string tratada em uma string para fazer o parsing
	str_stream = fmemopen (string, strlen(string), "r");

	//le o primeiro vertice da linha
	fscanf(str_stream, "%d", &new_line.v1);

	//salva o primeiro vertice (para a linha que liga o primeiro vertice com o ultimo)
	first = new_line.v1;

	//leitura de todas as linhas
	for (i = 0; i < size - 1; i++)
	{
		fscanf(str_stream, "%d", &new_line.v2);
		insert_line (lines, new_line, last_index);
		new_line.v1 = new_line.v2;
	}

	//insere a linha que liga o primeiro com o ultimo vertice
	new_line.v1 = first;
	insert_line (lines, new_line, last_index);
}

//funcao principal de leitura que chama
//as funcoes pro tratamento dos vertices e faces
void readobj(FILE *stream, vlist_t *vertices, linelist_t *lines)
{
	int last_index_vertices = 0;
	int last_index_lines = 0;
	char c;

	//inicialização dos dados
	vertices->size = 0;
	lines->size    = 0;
	vertices->array = NULL;
	lines->array    = NULL;

	//loop até o fim do arquivo
	while((c = fgetc (stream)) != EOF)
	{
		switch (c)
		{
			case 'v':
				c = fgetc (stream);
				
				//se a linha se tratar de qualquer coisa além de vertices (vt,vn,vp...)
				if (c != ' ')
					get_line (stream);
				else
					//leitura de vertices
					read_vertice (stream, vertices, &last_index_vertices);

				break;

			case 'f':
				c = fgetc (stream);
				
				//se a linha se tratar de qualquer coisa além de faces
				if (c != ' ')
					get_line (stream);
				else
					//leitura de faces
				    read_face (stream, lines, &last_index_lines);

				break;

			case '\n':
				break;
			
			default:
				get_line (stream);
				break;
		}
	}

	//redimensionamento final dos vetores para liberar espaço adicional desnecessario
	if (last_index_vertices < vertices->size)
	{
		vertices->size = last_index_vertices;
		vertices->array = (coord3_t*) realloc (vertices->array, sizeof(coord3_t) * vertices->size);
	}

	if (last_index_lines < lines->size)
	{
		lines->size = last_index_lines;
		lines->array = (line_t*) realloc (lines->array, sizeof(line_t) * lines->size);
	}
}
