#include <iostream>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>  
#include <math.h>
#include <vector>
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
	Matrix (const Matrix<T> &a)
	{
		n=a.n;
		m=a.m;
		data= new T*[n];
		for (int x=0;x<n;x++)
		{
			data[x]=new T[m];
		}

		for (int x=0;x<n;x++)
		{
			for (int y=0;y<m;y++)
			{
				data[x][y]=a.data[x][y];
			}
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
		cout<<"\n\n\n";
	}

	Matrix<T> operator + (Matrix<T> a){

		Matrix<T> res (a.n,a.m);
		for (int x=0;x<res.n;x++){
			for(int y=0;y<res.m;y++){
				res.data[x][y]=a.data[x][y]+data[x][y];

			}
		}
		return res;
	}

	Matrix<T> operator - (Matrix<T> a){

		Matrix<T> res (a.n,a.m);
		for (int x=0;x<res.n;x++){
			for(int y=0;y<res.m;y++){
				res.data[x][y]=data[x][y]-a.data[x][y];

			}
		}
		return res;
	}

	vector<Matrix<T> > Separate ()
	{
		vector<Matrix<T> > result;
		int u=n*m/4;
		int size= ceil(sqrt(u));
		
		Matrix<T> p1(size,size);
		for (int x=0;x<size;x++)
		{
			for (int y=0;y<size;y++)
			{
				p1.data[x][y]=data[x][y];
			}
		}
		Matrix<T> p2(size,size);
		int c1=0;
		int c2=0;
		for (int x=0;x<size;x++)
		{
			c2=0;
			for (int y=size;y<n;y++)
			{	
				p2.data[c1][c2]=data[x][y];
				c2++;
			}
			c1++;
		}
		Matrix<T> p3(size,size);
		c1=0;
		for (int x=size;x<n;x++)
		{
			c2=0;
			for (int y=0;y<size;y++)
			{
				p3.data[c1][c2]=data[x][y];
				c2++;
			}
			c1++;
		}
		Matrix<T> p4(size,size);
		 c1=0;
		 
		for (int x=size;x<n;x++)
		{
			c2=0;
			for (int y=size;y<n;y++)
			{
				p4.data[c1][c2]=data[x][y];
				c2++;
			}
			c1++;
		}
		result.push_back(p1);
		result.push_back(p2);
		result.push_back(p3);
		result.push_back(p4);
		return result;


	}

	Matrix<T> strassen (Matrix<T> a,Matrix<T> b)
	{
		

		int n=a.n;
		Matrix<T> res (n,n);
		vector<Matrix<T> > partition_res=res.Separate();
		cout<<n<<endl;
		if (n==1)
		{
			(res.data[0][0])=((a.data[0][0])) * ((b.data[0][0]));
		}
	
		else
		{
			vector<Matrix<T> > partition_a= a.Separate();
			vector<Matrix<T> > partition_b= b.Separate();
			partition_res[0]=strassen(partition_a[0],partition_b[0])+strassen(partition_a[1],partition_b[2]);
			partition_res[1]=strassen(partition_a[0],partition_b[1])+strassen(partition_a[1],partition_b[3]);
			partition_res[2]=strassen(partition_a[2],partition_b[0])+strassen(partition_a[3],partition_b[2]);
			partition_res[3]=strassen(partition_a[2],partition_b[1])+strassen(partition_a[3],partition_b[3]);

		}


		return res;

	}
	


};


int main()
{
	srand(time(NULL));
	Matrix <int> a(4,4);
	Matrix<int> b(4,4);
	a.FillWithRandom();
	b.FillWithRandom();
	
	a.ShowMatrix();
	cout<<"\n\n\n\n";
	b.ShowMatrix();
	a.strassen(a,b);

	
	return 0;
	
}