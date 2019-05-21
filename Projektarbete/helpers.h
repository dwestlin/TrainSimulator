#ifndef  HELPERS_H
#define HELPERS_H
#include <sstream>
#include <iostream>
#include <string>


// globala deklarationer.
const int PRE_TIME = 30;
const int ASSEMBLE_TIME = 20;
const int READY_TIME = 10;
const int DISASSEMBLE_TIME = 20;
const int STOP_TIME = 1440;
using namespace std;

class helpers
{
private:


public:
	//konstrukt�rer
	helpers() {}

	//g�r om fr�n str�ngen inl�sningen till integer.
	int fromStringtoInt(string &s);

	//anv�nds f�r att g�ra om till en str�ng vid presentation.
	string fromInttoString(int time);

};

#endif