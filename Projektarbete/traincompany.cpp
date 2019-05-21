#include "traincompany.h"
#include <string>
#include <iostream>

//Konstruktör, läser in alla filer samt schedular assembleEvent. anropar även simuleringen.
trainCompany::trainCompany() : theSim(new Simulation())
{
	readTrainStation();
	readTrains();
	readTrainMap();
	assembleAllTrains();
}

//Funktion som läser in tågtabellen.
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

			//skapar integers för att lägga in strängen och göra om tiden till integers, som underlättar simuleringen.
			int d = h.fromStringtoInt(dTime), ar = h.fromStringtoInt(aTime);


			// Skapar objektet för att kunna stoppa in i train-vectorn i traincompany och även i rätt station.
			shared_ptr<Train> tempTrain(new Train(id, dStation, aStation, d, ar, maxSpeed, veh));

			//skickar in objektet i en vector.
			trains.push_back(tempTrain);
			//adderar tågen på rätt station.
			stations[tempTrain->getDepStation()]->addTrain(tempTrain);


			//rensar vectorn så att nya fordonstyper kan föras in.
			veh.clear();
		}

	}
}

//Funktion som ska läsa in tågstationen samt vilka fordon som finns i fordonspoolen.
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


			//så länge det finns fordon att läsas in så skickar den in fordons-klasser.
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

//Funktion för att läsa in tågstationen och distancen mellan dom.
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


//överlagring för att dels få bort tecken som inte ska läsas in samt skickar tillbaka objekt för rätt vehicletype.
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

//funktion för att räkna ut avståndet mellan två stationen. Den går då in i en map-container och söker efter första stationen, därefter söker den efter arrivingStation och får därför ett värde.
double trainCompany::getDistance(std::string departureStation, std::string arrivingStation)
{
	return distances.find(departureStation)->second.find(arrivingStation)->second;
}


//Funktion för att testa bygga ihop fordonen.
bool trainCompany::tryBuildTrain(shared_ptr<Train> &train)
{
	//skapar en vector för att hämta tågets fordonstyper som behövs.
	vector<int> typeOfVehicle = train->getTypeOfVehicles();
	int counter = 0;
	shared_ptr<vehicle> v;

	//går igenom alla fordonstyper som behövs
	for (auto idx : typeOfVehicle)
	{
		//Om fordonstypen finns på avgångsstationen, då addar den fordonet till tågets vector samt plussar på en räknar för att kontrollera så att alla typer som behövs läggs till.
		if (stations[train->getDepStation()]->requestCarriage(idx, v))
		{
			counter++;
			train->addVehicle(v);

		}

	}


	//en sista check för att se om alla fordon lagts in. Om så är fallet, då clearas vectorn för att göra plats till nästa ihopsättning.
	if (counter == typeOfVehicle.size())
	{
		typeOfVehicle.clear();
		return true;
	}
	typeOfVehicle.clear();
	return false;

}

//Funktion för att skapa ett assembleEvent för ALLA fordon som finns i tågtabellen. 
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

//Funktion för att skriva ut när den är assembled.
void trainCompany::assembleState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;






}


//funktion för att indikera att tåget är ready.
void trainCompany::readyState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;


}

//funktion för att skriva ut dels att det är i running state samt tar bort tåget från stationen och lägger in i runningTrain-vectorn på traincompany.
void trainCompany::runningState(shared_ptr<Train>& train)
{
	train->setDistance(getDistance(train->getDepStation(), train->getDestStation()));
	stations[train->getDepStation()]->removeTrain(train->getId());
	runningTrain(train);
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " with distance "  << train->getDistance() << " km" << endl;


}
//funktion för att skriva ut att tåget har arrived.
void trainCompany::ArrivedState(shared_ptr<Train>& train)
{
	cout << h.fromInttoString(theSim->getTime()) << " " << "[" << train->getId() << "]" << "(" << train->getState() << ")" << " from " << train->getDepStation() << " " << "(" <<
		h.fromInttoString(train->getDepartureTime()) << ")" << " to " << train->getDestStation() << " " << "(" << h.fromInttoString(train->getArrivingTime()) << ")" << " " << train->getMaxSpeed() << " km/h " << endl;


}
//finished funkion som först letar upp angivet tåg  efter id som skickas med och sen deletar det från RunningTrain vectorn och lägger in det på rätt station.
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
