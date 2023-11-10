#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int *BuildBCT(char *Search){
	int *Table = (int*)malloc(0x80);
	memset(Table, -1, 0x80);

	for(int i=0; Search[i]; i++){
		Table[Search[i]] = i;
	}

	return Table;
}

void BuildGST(char *Search, int *SuffixTable, int *DistanceTable){
	int i, j, SearchLength;

	(i = SearchLength = strlen(Search), j = i+1);

	SuffixTable[i] = j;

	while(i){
		while(j <= SearchLength && Search[i-1] != Search[j-1]){
			if(DistanceTable[j] == 0)
			{
				DistanceTable[j] = j-i;
			}

			j = SuffixTable[j];
		} 
		i--;
		j--;

		SuffixTable[i] = j;
	}

	j = SuffixTable[i];

	for(; i<=SearchLength ;){
		if(DistanceTable[i] == 0)
		{
			DistanceTable[i] = j;
		}

		if(i == j)
		{
			j = SuffixTable[j];
		}

		i++;
	}
}

void BoyerMoore(char *Origin, char *Search, int current, int *ret){
	int *BCT = NULL, *SuffixTable = NULL, *DistanceTable = NULL;
	int OriginLength = strlen(Origin), SearchLength = strlen(Search);
	int i = current, j = 0;
	*ret = -1;

	SuffixTable = (int*)calloc(SearchLength + 1, sizeof(int));
	DistanceTable = (int*)calloc(SearchLength + 1, sizeof(int));

	BCT = BuildBCT(Search);
	BuildGST(Search, SuffixTable, DistanceTable);

	while(i <= OriginLength - SearchLength){
		j = SearchLength - 1;
		while(j > -1 && Search[j] == Origin[i+j]){
			j--;
		}

		if(j <= -1){
			*ret = i;
			return;
		}else{
			#define Max(a,b) ((a) ^ ((a)^(b)) & -((a)<(b)))
			i += Max(DistanceTable[j+1], j - BCT[Origin[i+j]]);
		}
	}

	free(BCT);
	free(SuffixTable);
	free(DistanceTable);
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
		BoyerMoore(Origin, Search, 0, &ret);
		Line++;

		if(ret != -1){
			printf("line: %d, column: %d : %s", Line, ret+1, Origin);
		}
	}

	fclose(fp);

	return 0;
}
