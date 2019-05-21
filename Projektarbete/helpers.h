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
	//konstruktörer
	helpers() {}

	//gör om från strängen inläsningen till integer.
	int fromStringtoInt(string &s);

	//används för att göra om till en sträng vid presentation.
	string fromInttoString(int time);

};

#endif