#ifndef STATION_H
#define STATION_H

#include <string>
#include <memory>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "train.h"

using namespace std;

class VehicleComparison;


class station
{

protected:
	string stationName;
	//vector för att lagra tågen.
	vector<shared_ptr<Train>> trains;
	//map-container för att lagra fordonen och dess fordonstyper
	map<int, set<shared_ptr<vehicle>, VehicleComparison>> vehicles;
public:
	station();
	station(string name);
	string getName() { return stationName; }


	//addera tåg och fordon in på stationen.
	void addTrain(shared_ptr<Train> &train);
	void addVehicle(shared_ptr<vehicle> &vehicle);
	void showVehicle();


	//Används för att ta bort tåget när det ska åka iväg.
	void removeTrain(int id);
	//Kontrollerar om rätt vehicleType finns på stationen.
	bool requestCarriage(int vehicleType, shared_ptr<vehicle> &v);

};


//sorterar på lägst id först.
class VehicleComparison {
public:
	bool operator() (std::shared_ptr<vehicle> right,  std::shared_ptr<vehicle> left) {
		return left->getID() > right->getID();
	}
};

#endif