#include <iostream>
#include <memory>
#include <string>
#include <deque>
#include <fstream>
#include "traincompany.h"

#include "memstat.hpp"

using namespace std;
int main()
{
	//skapar objektet traincompany.
	trainCompany trainCompany;

	//run-funktionen som startar ig�ng menyn.
	trainCompany.run();


	system("pause");
	return 0;
	

}