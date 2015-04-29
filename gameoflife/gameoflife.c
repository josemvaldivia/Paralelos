#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"
#include <unistd.h>
#include <math.h>


struct Matrix
{
	bool** data;
	size_t rows;
	size_t cols;
};

struct Matrix CreateNewMatrix(size_t n, size_t m)
{
	struct Matrix mat;
	mat.rows=n;
	mat.cols=m;
	mat.data=calloc(n, sizeof(int*));
	for (size_t i=0;i<n;i++)
	{
		mat.data[i]=calloc(m,sizeof(int));
	}
	return mat;
}

void FillRandomMatrix(struct Matrix  mat)
{
	for (int i=0;i<mat.rows;i++)
	{
		for (int j=0;j<mat.cols;j++)
		{

				int num=rand()%1000;
				if (num<100)
				{
					mat.data[i][j]=true;
				}
				else
				{
					mat.data[i][j]=false;
				}
		}

	}
}

void ShowMatrix(struct Matrix mat)
{
	for (int i=0;i<mat.rows;i++)
	{
		for (int j=0;j<mat.cols;j++)
		{
			printf("%d\t",mat.data[i][j]);
		}
		printf("\n");
	}

}

void PrintVector(bool * vec,int size)
{

	for (int i=0;i<size;i++)
	{
		printf("%d,\t",vec[i]);
	}
	printf("\n\n");
}


bool* Neighbors(struct Matrix mat,int i, int j)
{
	
	bool * res= calloc(8,sizeof(bool));
	res[0]= mat.data[i][((j-1)<0)?(mat.cols-1):(j-1)];//izquierda
	res[1]= mat.data[i][(j+1)%mat.cols];//derecha
	res[2]= mat.data[(i+1)%mat.rows][j];//abajo
	res[3]= mat.data[((i-1)<0)?(mat.rows-1):(i-1)][j];//arriba
	res[4]= mat.data[(i+1)%mat.rows][(j+1)%mat.cols];//derecha abajo
	res[5]=mat.data[((i-1)<0)?(mat.rows-1):(i-1)][(j+1)%mat.cols];//derecha arriba
	res[6]=mat.data[(i+1)%mat.cols][((j-1)<0)?(mat.cols-1):(j-1)];//izquierda abajo
	res[7]=mat.data[((i-1)<0)?(mat.rows-1):(i-1)][((j-1)<0)?(mat.cols-1):(j-1)];//izquierda arriba
	

	return res;

}

int NeighborsAlive(bool* vec_neighbors,int size)
{
	int count=0;
	for (int i=0;i<size;i++)
	{
		if(vec_neighbors[i])
			count++;
	}
	return count;
}

void GameOfLifePoint(struct Matrix mat,size_t pos_row,size_t pos_col)
{
	int neigh=NeighborsAlive(Neighbors(mat,pos_row,pos_col),8);
	
	if ((neigh>3)||(neigh<2))
	{
		mat.data[pos_row][pos_col]=false;
	}
	if (neigh ==3)
	{
		mat.data[pos_row][pos_col]=true;
	}

}

void GameOfLife(struct Matrix mat)
{
	for (int i=0;i<mat.rows;i++)
	{
		for (int j=0;j<mat.cols;j++)
		{
			GameOfLifePoint(mat,i,j);
		}
	}
}



void PrintMatrix(struct Matrix mat)
{
	for (int i=0;i<mat.rows;i++)
	{
		for (int j=0;j<mat.cols;j++)
		{
			if (mat.data[i][j])
			{
					_putpixel(i,j);
			}
		}
	}
}


int main ()
{ 
	srand(time(NULL));
	struct Matrix mat= CreateNewMatrix(600,600);
	FillRandomMatrix(mat);
	
	int gd, gm;
  
  	gd = X11;
  	gm = X11_1024x768;
  	initgraph (&gd, &gm, "");
  
  	setbkcolor (BLACK);
  	cleardevice ();
  	setcolor (WHITE);
  	for (int i=0;i<200;i++)
  	{
  		PrintMatrix(mat);
  		cleardevice();
  		GameOfLife(mat);
  		
 		 		
  	}
  	sleep(4);
  	closegraph ();
 

	return 0;
}
