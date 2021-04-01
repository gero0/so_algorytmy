#include "SJF_Preemptive.h"
#include "../Simulation.h"

namespace Scheduling {
	void SJF_Preemptive::update(Simulation* sim) {
		// Check if any new processes arrived and move them in the ready queue
		for (auto it = sim->newProc.begin(); it != sim->newProc.end();) {
			if (it->arrivalTime <= sim->currentStep) {
				sim->readyProc.push_back(*it);
				it = sim->newProc.erase(it);
				//sort ready processes - shortest job goes first
				std::sort(sim->readyProc.begin(), sim->readyProc.end(),
					[](const Process& a, const Process& b) {
						return (a.burstTime - a.executionTime) < (b.burstTime - b.executionTime);
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
			//check if there is a shorter job available among ready processes
			else if (!sim->readyProc.empty()) {
				//since elements are already sorted, we need to check only the first process in the vector
				Process next = sim->readyProc.at(0);
				if ((next.burstTime - next.executionTime) < (process.burstTime - process.executionTime)) {
					//"Preempty" current process and start executing the shortest job
					sim->readyProc.push_back(process);
					sim->currentProcess = next;
					sim->readyProc.erase(sim->readyProc.begin());
					//Sort the processes to make sure they're in correct order
					std::sort(sim->readyProc.begin(), sim->readyProc.end(),
						[](const Process& a, const Process& b) {
							return (a.burstTime - a.executionTime) < (b.burstTime - b.executionTime);
						}
					);
				}
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

		//incrementing waiting time for ready processes
		for (auto& process : sim->readyProc) {
			process.waitingTime++;
		}
	}
}