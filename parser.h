#pragma once
using namespace std;
#include <string>
#include <vector>
#include "matrix.h"
class parser
{
private:
		vector <matrix*> m;
		string removeAllSpaces(string s);
		string removeFirstSpaces(string s);
		void split(string s,char separator, vector <string> &names);
		void split(string s,string separators,vector <string> &names,vector <char> &operations);
public:
	int getSize();
	matrix * addMatrix(string x);
	matrix * findMatrix(string x);
	matrix * findOrAdd(string x);
	int getIndex(string s);
	void declareMatrix(string s);
	matrix* calculateSingleOperation(char c, matrix* p1, matrix* p2);
	string parseSingleExpression(string s);
	string parseExpression(string s);
	void calculateMatrix(string s);
};

