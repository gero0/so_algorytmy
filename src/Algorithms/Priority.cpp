#include "Priority.h"
#include "../Simulation.h"

namespace Scheduling {
	Priority::Priority(float agingRate = 0.5f) {
		this->agingRate = agingRate;
	}

	Priority::Priority() {
		std::cout << "Tempo postarzania (float >= 0): ";
		float input = -1;
		std::cin >> input;
		if (input < 0 || std::cin.fail()) {
			std::cout << "Nieprawidlowa wartosc\n";
			throw std::runtime_error("Nie podano prawidlowych parametrow wymaganych przez algorytm\n");
		}
		agingRate = input;
	}

	void Priority::update(Simulation* sim) {
		// Check if any new processes arrived and move them in the ready queue
		for (auto it = sim->newProc.begin(); it != sim->newProc.end();) {
			if (it->arrivalTime <= sim->currentStep) {
				sim->readyProc.push_back(*it);
				it = sim->newProc.erase(it);
				//sort ready processes by priorities
				//if priorities are equal sort sjf
				std::sort(sim->readyProc.begin(), sim->readyProc.end(),
					[](const Process& a, const Process& b) {
						if (a.priority < b.priority) {
							return true;
						}
						else if(a.priority == b.priority){
							return a.burstTime < b.burstTime;
						}
						else {
							return false;
						}
					}
				);
			}
			else {
				it++;
			}
		}

		//check if current process is not empty
		if (sim->currentProcess.has_value()) {
			Process& process = sim->currentProcess.value();
			//check if process finished execution.If so, move to finished processes and clear currentProcess.
			if (process.executionTime == process.burstTime) {
				process.completionTime = sim->currentStep;
				sim->finishedProc.push_back(process);
				sim->currentProcess.reset();
			}
		}

		//If there is no process curently executing choose the first one on the ready list.
		if (!sim->currentProcess.has_value() && !sim->readyProc.empty()) {
			sim->currentProcess = sim->readyProc[0];
			sim->readyProc.erase(sim->readyProc.begin());
		}

		//"executing" current process
		if (sim->currentProcess.has_value()) {
			sim->currentProcess.value().executionTime++;
		}

		//"Aging" awaiting processes
		for (auto& process : sim->readyProc) {
			process.priority -= agingRate;
			// priority can't go lower than 0
			if (process.priority < 0)
				process.priority = 0;
		}

		//incrementing waiting time for ready processes
		for (auto& process : sim->readyProc) {
			process.waitingTime++;
		}
	}
}