/*#ifndef VEHICLEFACTORY_H
#define VEHICLEFACTORY_H

//#include "train.h"
#include "vehicle.h"
#include <vector>
#include <iostream>

using namespace std;

class vehicleFactory
{

public:
	vehicleFactory() {};
    ~vehicleFactory() {};

vehicle * buildVehicle(int id, int type, int parameter1, int parameter2);

};
#endif // !VEHICLEFACTORY_H

 vehicle *vehicleFactory::buildVehicle(int id, int type, int parameter1, int parameter2)
 {


	switch (type)
	{
	case 0: return new seatCarriage(id,type, parameter1, parameter2);
	case 1: return new bedCarriage(id,type, parameter1); 
	case 2: return new openCarriage(id,type, parameter1, parameter2);
	case 3: return new closedCarriage(id,type, parameter1);
	case 4: return new electricLocomotive(id,type, parameter1, parameter2);
	case 5: return new dieselLocomotive(id,type, parameter1, parameter2);
	}

	 return 0;
}*/


