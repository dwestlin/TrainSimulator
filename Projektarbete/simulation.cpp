#include "simulation.h"
#include "event.h"


//funktion f�r att s�tta in fler event i priority_queue.
void Simulation::scheduleEvent(shared_ptr<Event> event)
{
	eventQueue.push(event);
}

//funktionen f�r att k�ra ig�ng hela simuleringen.
void Simulation::run()
{

		while (!eventQueue.empty()) {
			shared_ptr<Event>nextEvent = eventQueue.top();
			eventQueue.pop();		
			currentTime = nextEvent->getTime();
			nextEvent->processEvent();
	
		}

}

