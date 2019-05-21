#include "traincompany.h"
#include <string>
#include <iostream>

//Konstrukt�r, l�ser in alla filer samt schedular assembleEvent. anropar �ven simuleringen.
trainCompany::trainCompany() : theSim(new Simulation())
{
	readTrainStation();
	readTrains();
	readTrainMap();
	assembleAllTrains();
}

//Funktion som l�ser in t�gtabellen.
void trainCompany::readTrains()
{
	std::ifstream file;
	file.open("Trains.txt");

	string line, depart, dStation, aStation, dTime, aTime;
	
	int id, type;
	double maxSpeed;
	if (!file.is_open())
	{
		throw runtime_error("Couldn't open the file");

	}
	else {
		while (getline(file, line))
		{
			vector<int> veh;
			istringstream iss(line);
			iss >> id >> dStation >> aStation >> dTime >> aTime >> maxSpeed;

			while (iss >> type)
			{
				veh.push_back(type);
			}

			//skapar integers f�r att l�gga in str�ngen och g�ra om tiden till integers, som underl�ttar simuleringen.
			int d = h.fromStringtoInt(dTime), ar = h.fromStringtoInt(aTime);


			// Skapar objektet f�r att kunna stoppa in i train-vectorn i traincompany och �ven i r�tt station.
			shared_ptr<Train> tempTrain(new Train(id, dStation, aStation, d, ar, maxSpeed, veh));

			//skickar in objektet i en vector.
			trains.push_back(tempTrain);
			//adderar t�gen p� r�tt station.
			stations[tempTrain->getDepStation()]->addTrain(tempTrain);


			//rensar vectorn s� att nya fordonstyper kan f�ras in.
			veh.clear();
		}

	}
}

//Funktion som ska l�sa in t�gstationen samt vilka fordon som finns i fordonspoolen.
void trainCompany::readTrainStation()
{
	ifstream inFile;
	inFile.open("TrainStations.txt"); 
	std::string name;
	if (!inFile.is_open())
	{
		throw runtime_error("Couldn't open the file");
	
	}
	else {

		while (inFile >> name) {
			stations[name] = std::shared_ptr<station>(new station(name));
			vehicle *pointer = nullptr;


			//s� l�nge det finns fordon att l�sas in s� skickar den in fordons-klasser.
			while (inFile >> pointer) {

				vehicles.push_back(std::shared_ptr<vehicle>(pointer));
				stations[name]->addVehicle(vehicles.back());

			}

			inFile.clear();
		}
	}
	inFile.clear(); 
	inFile.close(); 
}

//Funktion f�r att l�sa in t�gstationen och distancen mellan dom.
void trainCompany::readTrainMap()
{
	ifstream file;
	file.open("TrainMap.txt");

	std::string name, name2;
	double distance;
	if (!file.is_open())
	{
		throw runtime_error("Couldn't open the file");

	}
	else {
		while (file >> name >> name2 >> distance)
		{
			distances[name].insert(pair<string, double>{name2, distance});
			distances[name2].insert(pair<string, double>{name, distance});
		}


	}
}


//�verlagring f�r att dels f� bort tecken som inte ska l�sas in samt skickar tillbaka objekt f�r r�tt vehicletype.
ifstream & operator >> (std::ifstream & ifs, vehicle *& pointer)
{

	int type;
	int id, par1, par2 = 0;


	while (ifs.peek() == ' ' || ifs.peek() == '(') {
		ifs.ignore();
	}

	ifs >> id >> type >> par1;

	if (ifs.peek() != ')') {
		ifs >> par2;
	}

	switch (type)
	{
	case 0:
		pointer = new seatCarriage(id, type, par1, par2);
		break;
	case 1:
		pointer = new bedCarriage(id, type, par1);
		break;
	case 2:
		pointer = new openCarriage(id, type, par1, par2);
		break;
	case 3:
		pointer = new closedCarriage(id, type, par1);
		break;
	case 4:
		pointer = new electricLocomotive(id, type, par1, par2);
		break;
	case 5:
		pointer  = new dieselLocomotive(id, type, par1, par2);
		break;
	}

	ifs.get();

	return ifs;

}

//funktion f�r att r�kna ut avst�ndet mellan tv� stationen. Den g�r d� in i en map-container och s�ker efter f�rsta stationen, d�refter s�ker den efter arrivingStation och f�r d�rf�r ett v�rde.
double trainCompany::getDistance(std::string departureStation, std::string arrivingStation)
{
	return distances.find(departureStation)->second.find(arrivingStation)->second;
}


//Funktion f�r att testa bygga ihop fordonen.
bool trainCompany::tryBuildTrain(shared_ptr<Train> &train)
{
	//skapar en vector f�r att h�mta t�gets fordonstyper som beh�vs.
	vector<int> typeOfVehicle = train->getTypeOfVehicles();
	int counter = 0;
	shared_ptr<vehicle> v;

	//g�r igenom alla fordonstyper som beh�vs
	for (auto idx : typeOfVehicle)
	{
		//Om fordonstypen finns p� avg�ngsstationen, d� addar den fordonet till t�gets vector samt plussar p� en r�knar f�r att kontrollera s� att alla typer som beh�vs l�ggs till.
		if (stations[train->getDepStation()]->requestCarriage(idx, v))
		{
			counter++;
			train->addVehicle(v);

		}

	}


	//en sista check f�r att se om alla fordon lagts in. Om s� �r fallet, d� clearas vectorn f�r att g�ra plats till n�sta ihops�ttning.
	if (counter == typeOfVehicle.size())
	{
		typeOfVehicle.clear();
		return true;
	}
	typeOfVehicle.clear();
	return false;

}

//Funktion f�r att skapa ett assembleEvent f�r ALLA fordon som finns i t�gtabellen. 
void trainCompany::assembleAllTrains()
{
	for (auto &train : trains)
	{
		theSim->scheduleEvent(std::shared_ptr<Event>(new AssembleEvent(theSim, this, train, train->getDepartureTime() - PRE_TIME)));
	}


}


//presenterar statistiken.
void trainCompany::statistic()
{
	cout << endl << endl << " Trains that was completed" << endl;
	for (auto train : finishedTrains)
	{
		cout << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
			h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << endl;
	}

	cout << endl << " Trains that never left the station: " << endl;
	for (auto train : incompletedTrains)
	{
		cout  << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
			h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" <<  endl;
	}
}

void trainCompany::printTrain()
{
	int id;
	cout << "please input trainID to check what kind of carriages it has" << endl;
	cin >> id;

	for (auto train : trains)
	{
		if (train->getId() == id)
		{
			if (train->getState() == FINISHED)
			{
				cout << "It has arrived to it's destination " << train->getDestStation() << ". No carriages found at the train" << endl;
			}
			else {
				vector<shared_ptr<vehicle>> vehicle = train->getCarriages();

				for (auto idx : vehicle)
				{
					cout << "trainID " << idx->getID() << " with vehicleType: ";
					if (idx->getVechicleType() == 0)
					{
						cout << "SEAT CARRAGE" << endl;
					}
					else if (idx->getVechicleType() == 1)
					{
						cout << "BED CARRAGE" << endl;
					}
					else if (idx->getVechicleType() == 2)
					{
						cout << "OPEN CARRAGE" << endl;
					}
					else if (idx->getVechicleType() == 3)
					{
						cout << "CLOSED CARRAGE" << endl;
					}
					else if (idx->getVechicleType() == 4)
					{
						cout << "ELECTRIC LOCOMOTIVE" << endl;
					}
					else if (idx->getVechicleType() == 5)
					{
						cout << "DIESEL LOCOMOTIVE" << endl;
					}

				}
			}
			break;
		}

	}
	
}

void trainCompany::printStation()
{
	cin.get();
	string stationName;
	cout << "Please input stationName to check what kind of carriages it has" << endl;
	getline(cin, stationName);

	for (auto idx : stations)

		if (stationName == idx.first)
		{
			stations[stationName]->showVehicle();
			break;
		}
}



void trainCompany::run()
{



	int number;
	bool stats = false;
	do
	{
	
		cout << " ******* MENU ********" << endl;
		cout << " *** TRAIN COMPANY ***" << endl;
		cout << " 1. Start simulation" << endl;
		cout << " 2. Print statistik" << endl;
		cout << " 3. Print train info" << endl;
		cout << " 4. Print station info" << endl;
		cout << " 5. Quit Program" << endl;
		cout << " *********************" << endl;
		cin >> number;

		switch (number) {
		case 1:
			simRun();
			stats = true;
			break;
		case 2:
			if (stats == false)
			{
				cout << "you need to start simulation first" << endl;
			}
			else
			statistic();
			break;
		case 3:
			if (stats == false)
			{
				cout << "you need to start simulation first" << endl;
			}
			else
			printTrain();
			break;
		case 4:
			if (stats == false)
			{
				cout << "you need to start simulation first" << endl;
			}
			else
			printStation();
			break;
		case 5:
			break;
		default:
			cout << "wrong input" << endl;
		}

	} while (number != 5);
}

//Funktion f�r att skriva ut n�r den �r assembled.
void trainCompany::assembleState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;






}


//funktion f�r att indikera att t�get �r ready.
void trainCompany::readyState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;


}

//funktion f�r att skriva ut dels att det �r i running state samt tar bort t�get fr�n stationen och l�gger in i runningTrain-vectorn p� traincompany.
void trainCompany::runningState(shared_ptr<Train>& train)
{
	train->setDistance(getDistance(train->getDepStation(), train->getDestStation()));
	stations[train->getDepStation()]->removeTrain(train->getId());
	runningTrain(train);
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " with distance "  << train->getDistance() << " km" << endl;


}
//funktion f�r att skriva ut att t�get har arrived.
void trainCompany::ArrivedState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;


}
//finished funkion som f�rst letar upp angivet t�g  efter id som skickas med och sen deletar det fr�n RunningTrain vectorn och l�gger in det p� r�tt station.
void trainCompany::FinishedState(int id, shared_ptr<Train> &train)
{
	auto it = find_if(runningTrains.begin(), runningTrains.end(), [&id](shared_ptr<Train> &t) -> bool { return t->getId() == id; });

	if (it != runningTrains.end()) {
		runningTrains.erase(it);
	}


	stations[train->getDestStation()]->addTrain(train);

	vector<shared_ptr<vehicle>> trainCarriages = train->getCarriages();

	for (auto idx : trainCarriages)
	{
		stations[train->getDestStation()]->addVehicle(idx);
	}

	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")"  << " " << train->getMaxSpeed() << " km/h " << endl;

	trainCarriages.clear();
}
