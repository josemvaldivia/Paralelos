#include <time.h>
//#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"
#include <unistd.h>
#include <math.h>
#include <pthread.h>
//#include <mpi.h>
#include <string.h>

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
	bool * dat = (bool*) calloc(n*m,sizeof(bool*));
	mat.data=(bool**)calloc(n, sizeof(bool*));
	for (size_t i=0;i<n;i++)
	{
		mat.data[i]=&dat[i*mat.cols];
	}
	return mat;
}

void FillRandomMatrix(struct Matrix  mat)
{
	for (int i=0;i<mat.rows;i++)
	{
		for (int j=0;j<mat.cols;j++)
		{
				mat.data[i][j]=rand()%2;
			
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
	
	bool * res= (bool*)calloc(8,sizeof(bool));
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
	
	if (mat.data[pos_row][pos_col])
	{
		if ((neigh<2)|| (neigh>3))
		{
			mat.data[pos_row][pos_col]=false;
		}
	}
	else
	{
		if (neigh==3)
		{
			mat.data[pos_row][pos_col]=true;
		}
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

bool** GameOfLifeFromTo(struct Matrix mat, int from, int to)
{	
	bool* d= (bool*)calloc(mat.cols*(to-from),sizeof(bool));
	bool** data;
	data= (bool**)calloc(to-from,sizeof(bool*));
	for (int i=0;i<to-from;i++)
	{
		data[i]=&d[i*mat.cols];
	}
	int count=0;
	for (int i=from;i<to;i++)
	{
		for(int j=0;j<mat.cols;j++){
			GameOfLifePoint(mat,i,j);
			data[count][j]=mat.data[i][j];
			
		}
		
		count++;
	}
	return data;
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


void printSub(bool** data,size_t rows, size_t cols)
{
	printf("%d\t%d\n",(int)rows,(int)cols);
	for(size_t i=0;i<rows;i++)
	{
		for (size_t j=0;j<cols;j++)
		{
			
			printf("%d\t",data[i][j]);
		}
		printf("\n");
	}
}


int main ()
{ 
	int g_rows,g_cols;
	g_rows=600;
	g_cols=600;
	srand(time(NULL));
	struct Matrix mat= CreateNewMatrix(g_rows,g_cols);
	FillRandomMatrix(mat);

	int gd, gm;
  
  	gd = X11;
  	gm = X11_1024x768;
  	initgraph (&gd, &gm, "");
  
  	setbkcolor (BLACK);
  	cleardevice ();
  	setcolor (WHITE);
  
  	for (int i=0;i<50;i++)
  	{
  		PrintMatrix(mat);
  		cleardevice();
  		GameOfLife(mat);
  		
  	}
  	sleep(3);
  	closegraph ();
 	 

	int        comm_sz;               
	int        my_rank;               
	int local_from,local_to,local_n;
	local_n=mat.rows/comm_sz;
	local_from=my_rank*local_n;
	local_to= local_from+local_n;



	return 0;
}
