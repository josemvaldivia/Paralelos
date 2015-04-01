#include <iostream>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>  
using namespace std;

template <typename T>
class Matrix
{

public:
	T** data;
	int n,m;
	Matrix(int a, int b){
		n=a;
		m=b;
		data= new T*[a];
		for(int x=0;x<a;x++)
		{
			data[x]=new T[b];
		}
		
	}
	~Matrix()
	{


	}
	void FillWithRandom(){

		for (int x=0;x<n;x++)
		{

			for (int y=0;y<m;y++)
			{
				
				data[x][y]=rand()%100+1;
			}
		}
	}

	void ShowMatrix(){

		for (int x=0;x<n;x++)
		{
			for (int y=0;y<m;y++)
			{
				cout<<data[x][y]<<"\t";
			}
			cout<<"\n";
		}
	}

	void sum(Matrix a, Matrix b){

		Matrix res (a.n,a.m);
		for (int x=0;x<res.n;x++){
			for(int y=0;y<res.m;y++){
				res[x][y]=a[x][y]+b[x][y];

			}
		}
	}


};

template<typename T>
Matrix<T> Strassen(Matrix<T> mat){
	int division=mat.n/2;



}

int main()
{
	srand(time(NULL));
	Matrix <int> a(4,4);
	a.FillWithRandom();
	a.ShowMatrix();
	return 0;
	
}