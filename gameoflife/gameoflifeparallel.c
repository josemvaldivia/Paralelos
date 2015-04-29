#include <time.h>
//#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"
#include <unistd.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
typedef int bool;
#define false 0
#define true 1

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
	int * dat = calloc(n*m,sizeof(int*));
	mat.data=calloc(n, sizeof(int*));
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
	bool* d= calloc(mat.cols*(to-from),sizeof(bool));
	bool** data;
	data= calloc(to-from,sizeof(bool*));
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
	g_rows=4;
	g_cols=4;
	srand(time(NULL));
	struct Matrix mat= CreateNewMatrix(g_rows,g_cols);
	FillRandomMatrix(mat);
	/*

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
  		sleep(0.7);
  	}
  	sleep(1);
  	closegraph ();
 	*/ 

	int        comm_sz;               
	int        my_rank;               
	int local_from,local_to,local_n;
	local_n=mat.rows/comm_sz;
	local_from=my_rank*local_n;
	local_to= local_from+local_n;

	MPI_Init(NULL, NULL); 
	struct Matrix m = CreateNewMatrix(g_rows,g_cols);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

      local_n=mat.rows/comm_sz;
      local_from=my_rank*local_n;
      local_to= local_from+local_n;
      
      bool** data=NULL;
      
      m.data=NULL;
      if (my_rank != 0) {
          data=GameOfLifeFromTo(mat,local_from,local_to);
         printSub(data,local_n,g_cols);
         //MPI_Send(&data[0][0], local_n*mat.cols , MPI_INT, 0, 0,MPI_COMM_WORLD); 
         /* Create message */
         //printSub(data,local_n,mat.cols);
         MPI_Gather(data[0],local_n*g_cols,MPI_INT,m.data[0],g_cols*g_rows,MPI_INT,0,MPI_COMM_WORLD);
         //printf("\n\n\n");
         //sprintf(greeting, "Proceso %d, ira desde %d hacia %d", my_rank,local_from, local_to);
         /* Send message to process 0 */
         //MPI_Send(data, sizeof(data), MPI_INT, 0, 0,MPI_COMM_WORLD); 
      } 
      else {
        data=GameOfLifeFromTo(mat,local_from,local_to);  
        
        MPI_Gather(data[0],local_n*g_cols,MPI_INT,m.data[0],g_cols*g_rows,MPI_INT,0,MPI_COMM_WORLD);
    	printSub(data,local_n,g_cols);	
         /* Print my message */
         //printSub(data,local_n,mat.cols);
         //ShowMatrix(mat);
         //printf("\n\n\n");
         //printf("Proceso %d,  ira desde %d hacia %d\n", my_rank, local_from,local_to);
      }

      /* Shut down MPI */
      MPI_Finalize(); 

 	


	return 0;
}
