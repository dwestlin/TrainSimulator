#include "train.h"

//konstruktor
Train::Train(int trainid, string depart, string dest,int sTime, int eTime, double maxspeed,vector<int> &veh)
{

	id = trainid;
	depStation = depart;
	destStation = dest;
	startTime = sTime;
	endingTime = eTime;
	maxSpeed = maxspeed;

	
	typeOfVehicle = veh;

	
}

//adderar in fordonen i en vector.
void Train::addVehicle(shared_ptr<vehicle>& c)
{
	carriages.push_back(c);
}



//används vid utskrift för att skriva ut vad det är i för tillstånd.
ostream & operator<<(ostream & as, const TrainState & st)
{
	switch (st)
	{
	case NOT_ASSEMBLED:
		as << "NOT ASSEMBLED";
		break;
	case INCOMPLETE:
		as << "INCOMPLETE";
		break;
	case ASSEMBLED:
		as << "ASSEMBLED";
		break;
	case READY:
		as << "READY";
		break;
	case RUNNING:
		as << "RUNNING";
		break;
	case ARRIVED:
		as << "ARRIVED";
		break;
	case FINISHED:
		as << "FINISHED";
		break;
	}
	return as;
}




