#pragma once
using namespace std;
#include <string>
class matrix
{
private:
	int rows, columns;
	double **elements;
	string name;
public:
	//essentials
	matrix();
	matrix(int rows,int columns);
	void copy(const matrix &x);
	matrix(const matrix &temp);
	//setters and getters
	void setRows(int rows);
	void setColumns(int columns);
	void setName(string name);
	int getRows();
	int getColumns();
	string getName();
	//operators overloading
	matrix operator+(matrix &x);
	matrix operator-(matrix &x);
	matrix operator*(matrix &x);
	matrix inverse();
	double determinant();
	matrix getCofactor(int r,int c);
	matrix transpose();
	//matrix operator/(matrix &x);
	double* operator[](int x);
	void operator=(matrix &temp);
	//others
	void print();
	void destroy();
	~matrix();
};

