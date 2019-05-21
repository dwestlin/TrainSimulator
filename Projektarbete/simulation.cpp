#include "simulation.h"
#include "event.h"


//funktion för att sätta in fler event i priority_queue.
void Simulation::scheduleEvent(shared_ptr<Event> event)
{
	eventQueue.push(event);
}

//funktionen för att köra igång hela simuleringen.
void Simulation::run()
{

		while (!eventQueue.empty()) {
			shared_ptr<Event>nextEvent = eventQueue.top();
			eventQueue.pop();		
			currentTime = nextEvent->getTime();
			nextEvent->processEvent();
	
		}

}

