#include "RoundRobin.h"
#include "../Simulation.h"

namespace Scheduling {
	RoundRobin::RoundRobin(int quant) {
		timeQuant = quant;
	}

	RoundRobin::RoundRobin() {
		std::cout << "Kwant czasu (integer >= 1): ";
		int input = 0;
		std::cin >> input;
		if (input < 1 || std::cin.fail()) {
			std::cout << "Nieprawidlowa wartosc\n";
			throw std::runtime_error("Nie podano prawidlowych parametrow wymaganych przez algorytm\n");
		}
		timeQuant = input;
	}

	void RoundRobin::update(Simulation* sim) {

		// Check if any new processes arrived and move them in the ready queue
		for (auto it = sim->newProc.begin(); it != sim->newProc.end();) {
			if (it->arrivalTime <= sim->currentStep) {
				sim->readyProc.push_back(*it);
				it = sim->newProc.erase(it);
			}
			else {
				it++;
			}
		}

		//check if current process is not empty
		if (sim->currentProcess.has_value()) {
			Process& process = sim->currentProcess.value();
			//Check if process finished executing
			if (process.executionTime == process.burstTime) {
				process.completionTime = sim->currentStep;
				sim->finishedProc.push_back(process);
				sim->currentProcess.reset();
				//In this algorithm the readyProc vector is not sorted and order of processes does not change.
				//We want to start executing next process in the vector, so after removing one we need to decrement the index
				//because after removing an element of vector, following elements "shift left" by 1 position;
				currentProcessPosition--;
			}
			else if (timeCounter >= timeQuant) {
				//If the process has not finished, but its assigned time quant has finished, "preempty" the process
				sim->readyProc.insert(sim->readyProc.begin()+currentProcessPosition, process);
				sim->currentProcess.reset();
			}
		}

		//We can't just use else because state of currentProcess might have changed
		if(!sim->currentProcess.has_value()){
			currentProcessPosition++;
			//check if we reached the end of the queue, if so, go back to first element
			if (currentProcessPosition >= sim->readyProc.size())
				currentProcessPosition = 0;
			//start next process and reset time counter
			if (!sim->readyProc.empty()) {
				sim->currentProcess = sim->readyProc[currentProcessPosition];
				sim->readyProc.erase(sim->readyProc.begin() + currentProcessPosition);
			}
			timeCounter = 0;
		}

		//"executing" current process
		if (sim->currentProcess.has_value()) {
			sim->currentProcess.value().executionTime++;
			timeCounter++;
		}

		//incrementing waiting time for ready processes
		for (auto& process : sim->readyProc) {
			process.waitingTime++;
		}
	}
}