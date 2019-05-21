#ifndef SIMULATION_H
#define SIMULATION_H
#include <iostream>
#include <queue>
#include <vector>
#include "event.h"

using namespace std;

class Simulation
{
protected:

	priority_queue<shared_ptr<Event>, vector<shared_ptr<Event>>, EventComparison> eventQueue;
	int currentTime; // tiden för det senaste hanterade eventet.

public:
	Simulation() : currentTime(0), eventQueue() { }


	int getTime() const { return currentTime; }
	void scheduleEvent(shared_ptr<Event> newEvent);

	void run();


};


#endif