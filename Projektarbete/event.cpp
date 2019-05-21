#include "event.h"
#include "station.h"
#include "vehicle.h"
#include "train.h"
#include "traincompany.h"

AssembleEvent::AssembleEvent(shared_ptr<Simulation> pSimulator, trainCompany * pTrainCompany, shared_ptr<Train> pTrain, int time) : Event(time, pSimulator, pTrainCompany, pTrain) {}
ReadyEvent::ReadyEvent(shared_ptr<Simulation> pSimulator, trainCompany * pTrainCompany, shared_ptr<Train> pTrain, int time) : Event(time, pSimulator, pTrainCompany, pTrain) {}
RunningEvent::RunningEvent(shared_ptr<Simulation> pSimulator, trainCompany * pTrainCompany, shared_ptr<Train> pTrain, int time) : Event(time, pSimulator, pTrainCompany, pTrain) {}
ArriveEvent::ArriveEvent(shared_ptr<Simulation> pSimulator, trainCompany * pTrainCompany, shared_ptr<Train> pTrain, int time) : Event(time, pSimulator, pTrainCompany, pTrain) {}
FinishSimulation::FinishSimulation(shared_ptr<Simulation> pSimulator, trainCompany * pTrainCompany, shared_ptr<Train> pTrain, int time) : Event(time, pSimulator, pTrainCompany, pTrain) {}

void AssembleEvent::processEvent()
{

	if (pTrainCompany->tryBuildTrain(train))
	{
		train->setState(ASSEMBLED);
		pTrainCompany->assembleState(train);
		simulator->scheduleEvent(shared_ptr<Event>(new ReadyEvent(simulator,pTrainCompany, train, time + ASSEMBLE_TIME)));

	}
	else
	{
		train->setState(INCOMPLETE);
		pTrainCompany->incompletedTrain(train);
	}
}


void ReadyEvent::processEvent()
{
	train->setState(READY);
	pTrainCompany->readyState(train);
	simulator->scheduleEvent(shared_ptr<Event>(new RunningEvent(simulator, pTrainCompany, train, time + READY_TIME)));

}

void RunningEvent::processEvent()
{
	train->setState(RUNNING);
	pTrainCompany->runningState(train);
	time += train->getArrivingTime() - train->getDepartureTime();
	simulator->scheduleEvent(shared_ptr<Event>(new ArriveEvent(simulator, pTrainCompany, train, time)));
}

void ArriveEvent::processEvent()
{
	train->setState(ARRIVED);
	pTrainCompany->ArrivedState(train);
	simulator->scheduleEvent(shared_ptr<Event>(new FinishSimulation(simulator, pTrainCompany, train, time + DISASSEMBLE_TIME)));
}

void FinishSimulation::processEvent()
{
	train->setState(FINISHED);
	pTrainCompany->FinishedState(train->getId(), train);
	pTrainCompany->finishedTrain(train);
}
