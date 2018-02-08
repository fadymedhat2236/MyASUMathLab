#include "stdafx.h"
#include "parser.h"
#include <iostream>
using namespace std;
unsigned staticCounter = 0;
string parser::removeAllSpaces(string s)
{
	for (unsigned int i = 0;i < s.length();i++)
	{
		if (s[i] == ' ')
			s.erase(i, 1);
	}
	return s;
}

string parser::removeFirstSpaces(string s)
{
	while (s[0] == ' ')
		s.erase(0, 1);
	return s;
}
void parser::split(string s, char separator, vector <string> &names)
{
	while (s.find(separator) != -1)
	{
		int pos = s.find(separator);
		string temp = s.substr(0, pos);
		names.push_back(temp);
		int counter = pos + 1;
		while (s[counter] == separator)
			counter++;
		s = s.substr(counter);
	}
	names.push_back(s);
}
void parser::split(string s, string separators, vector <string> &names, vector <char> &operations)
{
	vector <int> places;
	for (unsigned int i = 0;i < s.length();i++)
	{
		for (unsigned int j = 0;j < separators.length();j++)
		{
			if (s[i] == separators[j])
			{
				places.push_back(i);operations.push_back(s[i]);break;
			}
		}
	}
	for (unsigned int i = 0;i < places.size();i++)
	{
		if (i == 0)
		{
			names.push_back(s.substr(0, places[i]));
		}
		else
		{
			names.push_back(s.substr(places[i - 1] + 1, places[i] - places[i - 1] - 1));
		}
	}
	if (places.size() == 0)
	{
		names.push_back(s);
	}
	else
		names.push_back(s.substr((places[places.size() - 1]) + 1));
	return;
}
int parser::getSize()
{
	return m.size();
}
matrix * parser::addMatrix(string x)
{
	matrix*p = new matrix();
	p->setName(x);
	m.push_back(p);
	return p;
}
matrix* parser::findMatrix(string x)
{
	for (unsigned int i = 0;i < m.size();i++)
	{
		if (m[i]->getName() == x)
		{
			return m[i];break;
		}
	}
	return NULL;
}
matrix* parser::findOrAdd(string x)
{
	matrix* p = this->findMatrix(x);
	if (p != NULL)
		return p;
	else
	{
		return this->addMatrix(x);
	}
}
int parser::getIndex(string s)
{
	for (unsigned int i = 0;i < m.size();i++)
	{
		if (m[i]->getName() == s)
			return i;
	}
	return - 1;
}
void parser::declareMatrix(string s)
{
	matrix *p;
	int pos = (s.find('='));
	string name = s.substr(0, pos);
	name = removeAllSpaces(name);
	bool toBePrinted;
	if ((s[s.length() - 1]) == ';')
		toBePrinted = 0;
	else
		toBePrinted = 1;
	int first, last;
	first = s.find('[');last = s.find(']');
	s = s.substr(first + 1, last - first - 1);
	s = removeFirstSpaces(s);
	vector <string> r;vector <string> numbers;
	split(s, ';', r);
	for (unsigned int i = 0;i < r.size();i++)
		split(r[i], ' ', numbers);
	p = new matrix(r.size(), (numbers.size() / r.size()));
	p->setName(name);
	int counter = 0;
	for (int i = 0;i < p->getRows();i++)
	{
		for (int j = 0;j < p->getColumns();j++)
		{
			(*p)[i][j] = atof(numbers[counter].c_str());
			counter++;
		}
	}
	m.push_back(p);
	if (toBePrinted)
	{
		p->print();
	}

}
matrix* parser::calculateSingleOperation(char c, matrix* p1, matrix* p2)
{
	matrix * result = NULL;
	if (c == '+')
	{
		result = new matrix(p1->getRows(), p1->getColumns());
		*result = (*p1) + (*p2);
	}
	else if (c == '-')
	{
		result = new matrix(p1->getRows(), p1->getColumns());
		*result = (*p1) - (*p2);
	}
	else if (c == '*')
	{
		result = new matrix(p1->getRows(), p2->getColumns());
		*result = (*p1) * (*p2);
	}
	else if (c == '/')
	{
		matrix temp = (p2)->inverse();
		result = new matrix(p1->getRows(), temp.getColumns());
		*result = (*p1)*temp;
	}
	return result;
}
string parser::parseSingleExpression(string s)
{
	vector <string> names;vector <char> operations;
	split(s, "+-*/", names, operations);
	if (names.size() == 1 && names[0].find("'")!=-1 && names[0].find("inv")!=-1)
	{
		return names[0];
	}
	for (unsigned int i = 0;i < names.size();i++)
	{
		if (names[i].find("inv") != -1)
		{
			matrix *p1, *result;result = new matrix;
			string str = names[i].substr(3,s.length()-3);
			p1 = this->findMatrix(str);
			*result = p1->inverse();
			result->setName(to_string(staticCounter));
			names.erase(names.begin() + i);
			auto it = names.begin() + i;
			it = names.insert(it, to_string(staticCounter));
			staticCounter++;
			m.push_back(result);
			i = -1;
			continue;
		}
		if (names[i].find("'") != -1)
		{
			matrix *p1, *result;result = new matrix;
			string str = names[i].substr(0, names[i].length() - 1);
			p1 = this->findMatrix(str);
			*result = p1->transpose();
			result->setName(to_string(staticCounter));
			names.erase(names.begin() + i);
			auto it = names.begin() + i;
			it = names.insert(it, to_string(staticCounter));
			staticCounter++;
			m.push_back(result);
			i = -1;
			continue;
		}
	}
	for (unsigned int i = 0;i < operations.size();i++)
	{
		if (operations[i] == '*' || operations[i] == '/')
		{
			matrix *p1, *p2, *result;
			p1 = this->findMatrix(names[i]);p2 = this->findMatrix(names[i + 1]);
			result = this->calculateSingleOperation(operations[i], p1, p2);
			result->setName(to_string(staticCounter));
			names.erase(names.begin() + i);
			names.erase(names.begin() + i);
			operations.erase(operations.begin() + i);
			auto it = names.begin() + i;
			it = names.insert(it, to_string(staticCounter));
			staticCounter++;
			m.push_back(result);
			i = -1;
		}
	}
	for (unsigned int i = 0;i < operations.size();i++)
	{
		if (operations[i] == '+' || operations[i] == '-')
		{
			matrix *p1, *p2, *result;
			p1 = this->findMatrix(names[i]);p2 = this->findMatrix(names[i + 1]);
			result = this->calculateSingleOperation(operations[i], p1, p2);
			result->setName(to_string(staticCounter));
			names.erase(names.begin() + i);
			names.erase(names.begin() + i);
			operations.erase(operations.begin() + i);
			auto it = names.begin() + i;
			it = names.insert(it, to_string(staticCounter));
			staticCounter++;
			m.push_back(result);
			i = -1;
		}
	}
	return(names[0]);
}
string parser::parseExpression(string s)
{
	string temp1, temp2;vector <string> names;
	//split(s,'=',names);
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
		temp1 = s.substr(firstPos + 1, lastPos - firstPos - 1);
		temp2 = parseExpression(temp1);
		s.replace(firstPos, lastPos - firstPos + 1, temp2);
		goto label;
	}
	else
	{
		return this->parseSingleExpression(s);
	}
}
void parser::calculateMatrix(string s)
{
	vector <string> names;
	split(s,'=',names);
	string name = names[0];
	names[1] = removeAllSpaces(names[1]);
	bool tobePrinted;int pos = names[1].length() - 1;
	if (names[1][pos] == ';')
	{
		tobePrinted = 0;names[1].erase(pos, 1);
	}
	else
		tobePrinted = 1;
	name = removeAllSpaces(name);
	string returnedNumber = parseExpression(names[1]);
	matrix *p = new matrix;
	(*p) = *(m[m.size() - 1]);
	p->setName(name);
	int index = getIndex("0");
	for (unsigned int i = index;i < m.size();i++)
		m[i]->destroy();
	int counter = staticCounter ;
	while (counter != 0)
	{
		m.pop_back();counter--;
	}
	staticCounter = 0;
	m.push_back(p);
	if (tobePrinted)
		p->print();
}