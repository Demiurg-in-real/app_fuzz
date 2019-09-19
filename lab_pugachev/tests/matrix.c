#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
int umn(int **ut);
int main()
{
	int **a, schet=0;//[2][2]={{1,2},{3,4}};
	a=(int**)malloc(2*(sizeof(int*)));
	for (int i=0; i<2;i++)
	{
		a[i]=(int*)malloc(2*sizeof(int));
		for (int j=0; j<2;j++)
		{
			a[i][j]=schet;
			schet++;
			printf("%i ", a[i][j]);
		}
		printf("\n");
	}
	umn(a);
	return 0;
}
int umn(int **ut)
{
	int it[2][2]={{3,4},{5,6}};
	for (int i=0;i<2;i++)
	{
		for(int y=0; y<2;y++)
		{
			it[i][y]*=ut[i][y];
			printf("%i ", it[i][y]);
		}
		printf("\n");
	}
	return **it;
}
