#ifndef EVENT_H
#define EVENT_H

#include "station.h"
#include "train.h"
class Simulation;
class trainCompany;

class Event
{
protected:
	shared_ptr<Train> train;
	trainCompany *pTrainCompany;
	shared_ptr<Simulation> simulator;
	unsigned int time;
public:
	Event(unsigned int t, shared_ptr<Simulation> simulator, trainCompany * pTrainCompany, shared_ptr<Train> train) : time(t), train(train), pTrainCompany(pTrainCompany), simulator(simulator) { }
	virtual ~Event() { }

	//funktion som ska kunna skicka in ett nytt event-objekt så att tåget tar nästa steg i sin trainState.
	virtual void processEvent() = 0;
	//get time for event
	unsigned int getTime() const { return time; }
};


//klass-funktion som ska sortera på lägst tid först.
class EventComparison {
public:
	bool operator() (shared_ptr<Event> left, shared_ptr<Event> right) {
		return left->getTime() > right->getTime();
	}
};

//varje tillstånd har en egen klass.
class AssembleEvent : public Event {
protected:
	
public:
	AssembleEvent(shared_ptr<Simulation> pSimulator, trainCompany *pTrainCompany, shared_ptr<Train> pTrain, int time);
	virtual void processEvent();


};

//varje tillstånd har en egen klass.
class ReadyEvent : public Event {
protected:

public:
	ReadyEvent(shared_ptr<Simulation> pSimulator, trainCompany *pTrainCompany, shared_ptr<Train> pTrain, int time);
	virtual	void processEvent();


};

//varje tillstånd har en egen klass.
class RunningEvent : public Event {
protected:


public:
	virtual	void processEvent();
	RunningEvent(shared_ptr<Simulation> pSimulator, trainCompany *pTrainCompany, shared_ptr<Train> pTrain, int time);


};
//varje tillstånd har en egen klass.
class ArriveEvent : public Event {
protected:

public:
	virtual void processEvent();
	ArriveEvent(shared_ptr<Simulation> pSimulator, trainCompany *pTrainCompany, shared_ptr<Train> pTrain, int time);

};
//varje tillstånd har en egen klass.
class FinishSimulation : public Event {
protected:


public:
	virtual void processEvent();
	FinishSimulation(shared_ptr<Simulation> pSimulator, trainCompany *pTrainCompany, shared_ptr<Train> pTrain, int time);

};

#endif