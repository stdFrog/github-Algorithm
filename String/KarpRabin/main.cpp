#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pow(int a, int b)
{
	if(b == 0){return 1;}
	if(b == 1){return a;}
	return a * pow(a, b-1);
}

int Hash(char *String, int Length)
{
	int value = 0;
	for(int i=0; i < Length; i++)
	{
		value += String[i] * pow(2, Length - (1 + i));
	}
	return value;
}

int ReHash(char *String, int index, int Length, int PrevHash, int PrevCoef)
{
	if(index == 0){return PrevHash;}
	return String[index + (Length - 1)] + ((PrevHash - PrevCoef * String[index - 1]) * 2);
}

void KarpRabin(char *Origin, char *Search, int current, int *ret)
{
	int SearchLength = strlen(Search), OriginLength = strlen(Origin);
	int Coef = pow(2, SearchLength - 1);
	int OrgHash = Hash(Origin, SearchLength);
	int SearchHash = Hash(Search, SearchLength);

	*ret = -1;

	for(int i=0; i<OriginLength - SearchLength; i++)
	{
		OrgHash = ReHash(Origin, i, SearchLength, OrgHash, Coef);

		if(OrgHash == SearchHash)
		{
			int j=0;
			for(; j<SearchLength ;)
			{
				if(Origin[i + j] != Search[j++])
				{
					break;
				}
			}

			if(j >= SearchLength)
			{
				*ret = i;
				return;
			}
		}
	}
}

#define BUF_SIZE 512
int main(int argc, char *argv[])
{
	FILE *fp;

	char *Search, *Path, Origin[BUF_SIZE];
	int Line = 0, ret, SearchLength;

	if(argc < 3)
	{
		printf("Usage : %s <FilePath> <Search>\n", argv[0]);
		return -1;
	}

	Path = argv[1];
	Search = argv[2];

	SearchLength = strlen(Search);

	if((fp = fopen(Path, "r")) == NULL)
	{
		printf("Cannot open file : %s\n", Path);
		return -1;
	}

	for(; fgets(Origin, BUF_SIZE, fp) != NULL ;)
	{
		KarpRabin(Origin, Search, 0, &ret);
		Line++;

		if(ret != -1)
		{
			printf("line: %d, column: %d : %s", Line, ret+1, Origin);
		}
	}

	fclose(fp);

	return 0;
}
