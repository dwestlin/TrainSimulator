#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <memory>
#include <iostream>
#include <fstream>

using namespace std;


class vehicle
{

protected:
	int vehicleNumber;
	int vType;
	

public:
	vehicle() {};
	vehicle(int vNumber, int Type) {vehicleNumber = vNumber; vType = Type;	}
	~vehicle() {};



	//getters
	int getID() { return vehicleNumber; }
	int getVechicleType() { return vType; }

	//virtuella funktioner beroende på vilken subklass det är.
	virtual int getNumberSeats() { return 0; }
	virtual bool hasInternet() { return false; }
	virtual int getNumberBeds() { return 0; }
	virtual int getWeightCapacity() { return 0; }
	virtual int getSquareMeters() { return 0; }
	virtual int getCubicMeters() { return 0; }
	virtual int getMaxSpeed() { return 0;}
	virtual int getConsumption() { return 0; }
};


//subklass.
class seatCarriage : public vehicle
{

protected:
	int seatCap;
	int internet;


public:
	seatCarriage() : vehicle() {};
	seatCarriage(int vehicleNumber, int type, int seatCapacity, int hasInternet) : vehicle(vehicleNumber, type), seatCap(seatCapacity), internet(hasInternet)
	{};

	//egenskaperna som en sittvagn har.
	bool hasInternet() { if (internet == 1) { return true; } else { return false; }}
	int getNumberSeats() { return seatCap; }

};

//subklass.
class bedCarriage : public vehicle
{

protected:
	int bedCap;


public:
	bedCarriage() : vehicle() {};
	bedCarriage(int vehicleNumber, int type, int bedCapacity) : vehicle(vehicleNumber, type), bedCap(bedCapacity) {	};

	//egenskap som liggvagn har.
	int getNumberBeds()  { return bedCap; }

};

//subklass.
class openCarriage : public vehicle
{

protected:
	int weightCap;
	int squareMeter;
public:
	openCarriage() : vehicle() {};
	openCarriage(int vehicleNumber, int type, int weightCapacity, int squareMeters) : vehicle(vehicleNumber, type), weightCap(weightCapacity), squareMeter(squareMeters) {};

	//funktioner för egenskaperna som en öppen vagn har.
	int getWeightCapacity()  { return weightCap; }
	virtual int getSquareMeters(){ return squareMeter; }

};

//subklass.
class closedCarriage : public vehicle
{

protected:
	int cubicMeters;
public:
	closedCarriage() : vehicle() {};
	closedCarriage(int vehicleNumber, int type, int cubicMeter) : vehicle(vehicleNumber, type), cubicMeters(cubicMeter) {};

	//egenskap som en stängd vagn har.
	int getCubicMeters() { return cubicMeters; }

};

//subklass.
class dieselLocomotive : public vehicle
{

protected:
	int maxSpeed;
	int dieselConsumption;
public:
	dieselLocomotive() : vehicle() {};
	dieselLocomotive(int vehicleNumber, int type, int mSpeed, int consumption) : vehicle(vehicleNumber, type), maxSpeed(mSpeed), dieselConsumption(consumption) {};

	//egenskaperna som ett diesel lok har.
	int getMaxSpeed() { return maxSpeed; }
	int getConsumption()  { return dieselConsumption; }


};

//subklass.
class electricLocomotive : public vehicle
{

protected:
	int maxSpeed;
	int electricConsumption;

public:
	electricLocomotive() : vehicle() {};
	electricLocomotive(int vehicleNumber, int type, int mSpeed, int consumption) : vehicle(vehicleNumber, type), maxSpeed(mSpeed), electricConsumption(consumption) {};

	//egenskaperna som ett el-lok har.
	int getMaxSpeed()  { return maxSpeed; }
	int getConsumption() { return electricConsumption; }


};


#endif // !VEHICLE_H




