#ifndef TRAINCOMPANY_H
#define TRAINCOMPANY_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include "station.h"
#include "helpers.h"
#include "train.h"
#include "event.h"
#include "simulation.h"
#include "vehicle.h"



using namespace std;

class Simulation;


class trainCompany
{
protected:
	map<string, std::shared_ptr<station>> stations;
	vector<shared_ptr<vehicle>> vehicles;
	vector<shared_ptr<Train>> trains;
	vector<shared_ptr<Train>> runningTrains;
	vector<shared_ptr<Train>> finishedTrains;
	vector<shared_ptr<Train>> incompletedTrains;
	string departureStation;
	string arrivingStation;
	double distance;
	map<string, map<string, double>> distances;
	shared_ptr<Simulation> theSim;
	helpers h;
	vehicle v;
public:
	trainCompany();

	//inl�sning och skapar event-objekt d�r den f�rs�ker s�tta ihop t�gen.
	void readTrainStation();
	void readTrains();
	void readTrainMap();
	void assembleAllTrains();

	//F�rs�k till att bygga t�gen
	bool tryBuildTrain(shared_ptr<Train> &train);


	//funktioner som ska skriva ut n�r diverse tillst�nd �r klart. 
	void assembleState(shared_ptr<Train> &train);
	void readyState(shared_ptr<Train> &train);
	void runningState(shared_ptr<Train> &train);
	void ArrivedState(shared_ptr<Train> &train);
	void FinishedState(int id, shared_ptr<Train> &train);

	shared_ptr<station> getStation(string name) { return stations[name];}
	shared_ptr<Train> getTrain(int id) {return trains[id];}

	//f� fram distansen mellan stationer.
	double getDistance(std::string departureStation, std::string arrivingStation);

	//s�tter in i vectorer beroende p� om de ej lyckades byggas ihop, om de kom fram samt om det �r p� t�gr�lsen.
	void runningTrain(shared_ptr<Train> &train) { runningTrains.push_back(train); }
	void finishedTrain(shared_ptr<Train> &train) { finishedTrains.push_back(train); }
	void incompletedTrain(shared_ptr<Train> &train) { incompletedTrains.push_back(train); }

	//statistik samt f�r att b�rja simuleringen.
	void statistic();
	void printTrain();
	void printStation();
	void run();
	void simRun() { theSim->run(); }
};
ifstream & operator >> (std::ifstream & os, vehicle *& vPtr);


#endif
