#include <stdio.h>
#include <stdlib.h>

/* funcao que retorna a stream indicada pela linha de comando ou stdio se nao encontrar */
FILE* get_stream (char **argv)
{
	FILE* stream;

	if (argv[1] == NULL)
			stream = stdin;

	else
	{
		stream = fopen (argv[1], "r");
		if (stream == NULL)
		{
			perror ("Fail at opening file");
			abort();
		}
	}

	return stream;
}