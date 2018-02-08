#include "stdafx.h"
#include "matrix.h"
#include <iostream>
#include <vector>
using namespace std;
matrix::matrix()
{
	rows = columns = 0;
	elements = NULL;
}
matrix::matrix(int rows, int columns)
{
	this->rows = rows;this->columns = columns;
	elements = new double*[rows];
	for (int i = 0;i < rows;i++)
		elements[i] = new double[columns];
}
void matrix::copy(const matrix &x)
{
	if (x.elements != NULL)
	{
		this->rows = x.rows;
		this->columns = x.columns;
		elements = new double*[rows];
		for (int i = 0;i < rows;i++)
			elements[i] = new double[columns];
		for (int i = 0;i < rows;i++)
		{
			for (int j = 0;j < columns;j++)
				elements[i][j] = x.elements[i][j];
		}
	}
	else
	{
		rows = columns = 0;
		elements = NULL;
	}

}
matrix ::matrix(const matrix &temp)
{
	this->copy(temp);
}
void matrix::setRows(int rows)
{
	this->rows = rows;
}
void matrix::setColumns(int columns)
{
	this->columns = columns;
}
void matrix::setName(string name)
{
	this->name = name;
}
int matrix::getRows()
{
	return rows;
}
int matrix::getColumns()
{
	return columns;
}
string matrix::getName()
{
	return name;
}
void matrix::operator=(matrix &temp)
{
	if (elements)
		this->destroy();
	this->copy(temp);
}
matrix matrix::operator+(matrix &x)
{
	if ((rows != x.rows) || (columns != x.columns))
		throw"the addition operation can't be executed";
	matrix temp(rows,columns);
	for (int i = 0;i < rows;i++)
		for (int j = 0;j < columns;j++)
			temp.elements[i][j] = elements[i][j] + x.elements[i][j];
	return temp;
}
matrix matrix::operator-(matrix &x)
{
	if ((rows != x.rows) || (columns != x.columns))
		throw"the subtraction operation can't be executed";
	matrix temp(rows, columns);
	for (int i = 0;i < rows;i++)
		for (int j = 0;j < columns;j++)
			temp.elements[i][j] = elements[i][j] - x.elements[i][j];
	return temp;
}
double matrix::determinant()
{
	if (rows != columns)
		throw "the determinant can't be found";
	double result = 1;
	matrix m = *this;
	for (int i = 0;i < rows-1;i++)
	{
		for (int j = i+1;j < rows;j++)
		{
			if (m.elements[i][i] == 0)
			{
				int counter = i + 1;
				while (elements[counter][i] == 0)
				{
					counter++;
					if (counter == rows-1)
						return 0;
				}
				for (int l = 0;l < rows;l++)
				{
					double temp = m.elements[i][l];m.elements[i][l] = m.elements[counter][l];m.elements[counter][l] = temp;
				}
				result *= -1;
			}
			double temp = m.elements[j][i] / m.elements[i][i];
			for (int k = 0;k < rows;k++)
			{
				m.elements[j][k]-=temp*m.elements[i][k];
			}
		}
	}
	for (int i = 0;i < rows;i++)
		result*= m.elements[i][i];
	return result;
}
matrix matrix::getCofactor(int r,int c)
{
	matrix temp(this->rows-1, this->columns-1);
	for (int i = 0;i < rows-1;i++)
	{
		for (int j = 0;j < columns-1;j++)
		{
			int iC = (i<r) ? i:i+1 ;
			int iR = (j<c) ? j:j+1 ;
			temp.elements[i][j] = this->elements[iC][iR];
		}
	}
	return temp;
}
matrix matrix::transpose()
{
	matrix temp(this->columns, this->rows);
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < columns;j++)
		{
			temp.elements[j][i] = this->elements[i][j];
		}
	}
	return temp;
}
matrix matrix::inverse()
{
	if (this->rows != this->columns)
		throw "the inverse can't be calculated";
	double d = this->determinant();
	if (d == 0)
		throw "this is a singular matrix";
	matrix temp(this->rows, this->columns);int sign = 1;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < columns;j++)
		{
			temp.elements[i][j] = sign*(this->getCofactor(i,j).determinant());
			temp.elements[i][j] = temp.elements[i][j] / d;
			sign *= -1;
		}
	}
	return temp.transpose();
}
matrix matrix::operator*(matrix &x)
{
	if ((columns != x.rows))
		throw"the multiplication operation can't be executed";
	matrix temp(rows, x.columns);
	for (int i = 0;i < rows;i++)
		for (int j = 0;j < x.columns;j++)
		{
			double sum = 0;
			for (int k = 0;k < columns;k++)
			{
				sum += elements[i][k] * x.elements[k][j];
			}
			temp.elements[i][j] = sum;
		}
	return temp;
}
double* matrix::operator[](int x)
{
	return elements[x];
}
void matrix::print()
{
	vector <int> placesOfSeparations;
	placesOfSeparations.push_back(0);
	vector <int> maxElements;
	for (int i = 0;i<columns;i++)
	{
		unsigned int max = 0;
		for (int j = 0;j<rows;j++)
		{
			if ((to_string(elements[j][i])).length()>max)
				max = (to_string(elements[j][i])).length();
		}
		maxElements.push_back(max);
	}
	int max = 3;
	for (int i = 0;i<columns;i++)
	{
		if (max + maxElements[i] <= 80)
		{
			max += maxElements[i];
			max += 3;
		}
		else
		{
			placesOfSeparations.push_back(i);
			max = 3;
			max += maxElements[i] + 3;
		}
	}
	placesOfSeparations.push_back(columns);
	//printing
	cout << name << " = " << endl << endl;
	for (unsigned int i = 0;i<placesOfSeparations.size() - 1;i++)
	{
		if (placesOfSeparations[1] != columns)
			cout << "Columns " << placesOfSeparations[i] + 1 << " through " << placesOfSeparations[i + 1] << endl << endl;
		for (int j = 0;j<rows;j++)
		{
			cout << "   ";
			for (int k = placesOfSeparations[i];k<placesOfSeparations[i + 1];k++)
			{
				for (unsigned int l = 0;l<maxElements[k] - (to_string(elements[j][k])).length();l++)
					cout << " ";
				cout << elements[j][k];
				if (k != placesOfSeparations[i + 1] - 1)
					cout << "   ";
			}
			cout << endl;
		}
		cout << endl;
	}

}
void matrix::destroy()
{
	if (elements)
	{
		for (int i = 0;i < rows;i++)
			delete[] elements[i];
		delete[] elements;
		elements = NULL;columns = rows = 0;
	}
}
matrix::~matrix()
{
	this->destroy();
}
