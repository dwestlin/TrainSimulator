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
	//vector f�r att lagra t�gen.
	vector<shared_ptr<Train>> trains;
	//map-container f�r att lagra fordonen och dess fordonstyper
	map<int, set<shared_ptr<vehicle>, VehicleComparison>> vehicles;
public:
	station();
	station(string name);
	string getName() { return stationName; }


	//addera t�g och fordon in p� stationen.
	void addTrain(shared_ptr<Train> &train);
	void addVehicle(shared_ptr<vehicle> &vehicle);
	void showVehicle();


	//Anv�nds f�r att ta bort t�get n�r det ska �ka iv�g.
	void removeTrain(int id);
	//Kontrollerar om r�tt vehicleType finns p� stationen.
	bool requestCarriage(int vehicleType, shared_ptr<vehicle> &v);

};


//sorterar p� l�gst id f�rst.
class VehicleComparison {
public:
	bool operator() (std::shared_ptr<vehicle> right,  std::shared_ptr<vehicle> left) {
		return left->getID() > right->getID();
	}
};

#endif