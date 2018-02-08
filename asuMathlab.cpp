// asuMathlab.cpp : D efines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "matrix.h"
#include "parser.h"
using namespace std;
unsigned int counter = 0;
string parseParentheses(string &s)
{
	cout << s << endl;
	string temp1, temp2;
	label:
	int firstPos = s.find('(');int lastPos;
	if (firstPos != -1)
	{
		int openBrackets = 1;int closedBrackets = 0;
		for (unsigned int i = firstPos + 1;i < s.length();i++)
		{
			if (s[i] == '(')
				openBrackets++;
			if (s[i] == ')')
				closedBrackets++;
			if (openBrackets == closedBrackets)
			{
				lastPos = i;
				break;
			}
		}
		temp1 = s.substr(firstPos + 1, lastPos-firstPos - 1);
		temp2= parseParentheses(temp1);
		s.replace(firstPos,lastPos-firstPos+1,temp2);
		//function of the operation should be written here
		//the numbers of each parantheses should be replaces by the number of returned value from the opration
		goto label;
	}
	else
	{
		return to_string(counter++);
	}
}
int main(int argc, char const *argv[])
{
	/*
	parser p1;
	string s1, s2, s3;
	s1 = "A=[412 498 494;412 498 81;91 91 81]";s2 = "B=[1 1 1;2 2 2;3 3 3]";s3 = "C=[33 45 56;71 32 12;1 2 3]";
	p1.declareMatrix(s1);p1.declareMatrix(s2);p1.declareMatrix(s3);
	p1.calculateMatrix("D=((A-B)*(C-(inv(A))'))");
	return 0;
	*/
	if (argc > 1)
	{
		cout << "welcome" << endl;
	}
	else
	{
		try
		{
			parser p1;
			while (1)
			{
				string s;
				getline(cin, s);
				if (s.find('[') != -1)
				{
					p1.declareMatrix(s);
				}
				else
				{
					p1.calculateMatrix(s);
				}
			}
		}
		catch (char* c)
		{
			cout << c << endl;
		}
	}
	return 0;
}

