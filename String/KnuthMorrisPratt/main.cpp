#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void BuildBorder(char *Search, int *Border)
{
	int i = 0, j = -1, SearchLength = strlen(Search);

	Border[i] = j;

	while(i < SearchLength)
	{
		while(j > -1 && Search[i] != Search[j])
		{
			j = Border[j];
		}

		i++;
		j++;

		Border[i] = j;
	}
}

void KnuthMorrisPratt(char *Origin, char *Search, int current, int *ret)
{
	int OriginLength = strlen(Origin);
	int SearchLength = strlen(Search);
	int i = current, j = 0, *Border = NULL;
	Border = (int*)calloc(SearchLength + 1, sizeof(int));

	*ret = -1;

	BuildBorder(Search, Border);

	while(i < OriginLength)
	{
		while(j > -1 && Origin[i] != Search[j])
		{
			j = Border[j];
		}

		i++;
		j++;

		if(j >= SearchLength)
		{
			*ret = i - j;
			break;
		}
	}

	free(Border);
}

#define BUF_SIZE 512
int main(int argc, char* argv[])
{
	FILE *fp;

	char *Path, *Search, Origin[BUF_SIZE];
	int SearchLength, Line, ret;

	if(argc < 3){
		printf("Usage: %s <FilePath> <Search>\n", argv[0]);
		return 1;
	}

	Path = argv[1];
	Search = argv[2];

	SearchLength = strlen(Search);

	if((fp = fopen(Path, "r")) == NULL){
		printf("Cannot open file: %s\n", Path);
		return 1;
	}

	Line = 0;
	while(fgets(Origin, BUF_SIZE, fp) != NULL){
		KnuthMorrisPratt(Origin, Search, 0, &ret);
		Line++;

		if(ret != -1){
			printf("line: %d, column: %d : %s", Line, ret+1, Origin);
		}
	}

	fclose(fp);

	return 0;
}
