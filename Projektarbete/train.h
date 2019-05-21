#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "vehicle.h"
#include "helpers.h"


enum TrainState { NOT_ASSEMBLED , INCOMPLETE, ASSEMBLED, READY, RUNNING, ARRIVED, FINISHED};

using namespace std;




class Train
{

protected:
	vector<shared_ptr<vehicle>> carriages;
	vector<int> typeOfVehicle;
	int id;
	double maxSpeed;
	string depStation;
	string destStation;
	double distance;
	int startTime;
	int endingTime;
	TrainState state;


public:
	Train() {};
	Train(int trainid, string depart, string dest,int sTime, int eTime,double maxspeed,vector<int> &veh);

	//används för att addera in fordonen till tåget.
	void addVehicle(shared_ptr<vehicle> &c);

	//setters
	void setDistance(double dist) { distance = dist; }
	void setState(TrainState s) { state = s; }

	//getters
	vector<int> getTypeOfVehicles() { return typeOfVehicle; }
	vector<shared_ptr<vehicle>> getCarriages() { vector<shared_ptr<vehicle>> copy = carriages; carriages.clear(); return copy; }
	double getDistance() {  return distance; }
	double getMaxSpeed() { return maxSpeed; }
	int getId() { return id; }
	int getArrivingTime() { return endingTime; }
	int getDepartureTime() { return startTime; }
	string getDepStation() { return depStation; }
	string getDestStation() { return destStation; }
	TrainState getState() { return state; }

};

ostream & operator<<(ostream & as, const TrainState & st);



#endif