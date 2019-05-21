#include "station.h"

station::station()
{
}

//konstruktor
station::station(string name)
{
	stationName = name;
}

//sätter in fordonet i rätt fordonstyp i containern.
void station::addVehicle(shared_ptr<vehicle> &v)
{
	vehicles[v->getVechicleType()].insert(v);
}

void station::showVehicle()
{
	for (auto &v : vehicles) {

		for (auto &v2 : v.second) {
			cout << "Train ID " << v2->getID() << " with vehicleType ";
			if (v2->getVechicleType() == 0)
			{
				cout << "SEAT CARRAGE" << endl;
			}
			else if (v2->getVechicleType() == 1)
			{
				cout << "BED CARRAGE" << endl;
			}
			else if (v2->getVechicleType() == 2)
			{
				cout << "OPEN CARRAGE" << endl;
			}
			else if (v2->getVechicleType() == 3)
			{
				cout << "CLOSED CARRAGE" << endl;
			}
			else if (v2->getVechicleType() == 4)
			{
				cout << "ELECTRIC LOCOMOTIVE" << endl;
			}
			else if (v2->getVechicleType() == 5)
			{
				cout << "DIESEL LOCOMOTIVE" << endl;
			} 
		}
	}
}




//tar bort tåget från tåg-vectorn på stationen. 
void station::removeTrain(int id)
{


		auto it = find_if(trains.begin(), trains.end(), [&id](shared_ptr<Train> &t) -> bool { return t->getId() == id; });

		if (it != trains.end()) {
			trains.erase(it);
		}

}


//kontrollerar om vehicleType finns på stationen.
bool station::requestCarriage(int vehicleType, shared_ptr<vehicle> &ve)
{

	for (auto & v : vehicles) {

		for (auto &v2 : v.second) {
			if (vehicleType == v2->getVechicleType())
			{
				auto it = vehicles[v2->getVechicleType()].begin();
				ve = *it;
				vehicles[v2->getVechicleType()].erase(it);
				return true;
			}
		}
	

	}
	return false;
}

//sätter in tågen på stationen igen.
void station::addTrain(shared_ptr<Train> &train)
{
	trains.push_back(train);
}


