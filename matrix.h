#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <iostream>
using namespace std;

class CMatrix{
	int i;
	int j;
	int n;
	double **matrix;
	double **matrix_alloc(int, int);
	void matrix_free(double **, int, int);
public:
	class IndexOutOfRange{};
	class WrongDim{};

	CMatrix(int, int, double, double);
	CMatrix(fstream&);
	~CMatrix();
	CMatrix& operator= (CMatrix &temp);
	CMatrix& operator*= (const CMatrix &temp);
	friend CMatrix operator* (CMatrix, CMatrix);

	double read(int i, int j) const;
	void write(int i, int j, double x);
	

	friend ostream & operator << (ostream & s, const CMatrix & temp)
  {
		int i = 0;
		int j = 0;
		for(i = 0; i < temp.i; i++){
			for(j = 0; j < temp.j; j++){
				s << temp.read(i,j) << "  " ;
			}
			s << "\n";
		}   
		
    return s;
  }

		class Cref2											// Cref2
	{
		friend class CMatrix;
  	CMatrix& s;
  	int i;
		int j;
	public:
  	Cref2 (CMatrix& ss, int ii, int jj): s(ss), i(ii), j(jj) {};
 
 	 void operator= (double x) 
  	{
    	s.write(i,j,x);
   	};
	operator double()
	{
		return s.read(i,j);
	}
};



	class Cref1											// Cref1
	{
		friend class CMatrix;
  	CMatrix& s;
  	int i;
	public:
  	Cref1 (CMatrix& ss, int ii): s(ss), i(ii) {};
 
 	 	Cref2 operator [] (int jj) 
 	 	{
			if(jj > s.j) throw CMatrix::IndexOutOfRange();
  		return Cref2(s, i, jj);
  	}
	
	};
	Cref1 operator[](int ii)
	{
		if(ii > i) throw CMatrix::IndexOutOfRange();
		return Cref1(*this, ii);
	}

};




double** CMatrix::matrix_alloc(int ii, int jj){
	  int k = 0;
    double **matrix;
    matrix = new double*[ii];

    if(matrix == NULL){
        throw bad_alloc();
    }else{
        for(k=0; k<ii; k++){
            matrix[k] = new double[jj];
            
					if(matrix[k] == NULL){
    				throw bad_alloc();
        	}
				}
    }    
    return matrix;
}

void CMatrix::matrix_free(double **matrix, int i, int j){
	int k = 0;
	for(k=0; k<j; k++){
		delete [] matrix[k];	
	}
	delete [] matrix;
}

CMatrix::CMatrix(int i, int j, double a = 0.0, double b = 0.0)
{
	int k = 0;
	int l = 0;
	this->i = i;
	this->j = j;
	/*alokacja */
	matrix = matrix_alloc(i,j);

	// Przypisania 
    
	for(k = 0; k < i; k++){
		for(l = 0; l < j; l++){
			if(k == l)
				this->matrix[k][l] = a;
			else
				this->matrix[k][l] = b;
		}
	}

}
CMatrix::CMatrix(fstream & s)
{
	int k = 0;
	int l = 0;
	int toInt;
	double toDouble;

	s >> toInt;
	this->i = toInt;

	s >> toInt;		
	this->j = toInt;


	matrix = matrix_alloc(i,j);

	// Przypisania 
	for(k = 0; k < i; k++){
		for(l = 0; l < j; l++){
				s >> toDouble;
				this->matrix[k][l] = toDouble;
		}
	}

}
CMatrix::~CMatrix()
{
	//matrix_free(matrix, i,j);
	for(int k = 0; k < i; k++)
		delete [] matrix[k];
	delete [] matrix;
}


CMatrix& CMatrix::operator= (CMatrix &temp )
{
  return *this;
	
}

CMatrix& CMatrix::operator*= (const CMatrix &temp){
	return *this;
}

inline CMatrix
operator * (CMatrix s1, CMatrix s2)
{
	if(s1.j != s2.i) throw CMatrix::WrongDim();
	else{
		int k = 0;
		int l = 0;
		int m = 0;

		CMatrix temp(s1.i, s2.j);
			
		for(k = 0; k < s1.i; k++){
			for(l = 0; l < s2.j; l++){
					for(m = 0; m < s1.j; m++){
							temp.matrix[k][l] += s1.matrix[k][m] * s2.matrix[m][l];		
					} 
				}
		}
		return temp;
	}
}

double CMatrix::read(int i, int j) const
{
	return matrix[i][j];
}

void CMatrix::write(int i, int j, double x)
{
	matrix[i][j] = x;
}



#endif /* __MATRIX_H__ */
