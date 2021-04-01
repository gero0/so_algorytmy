#include "SJF.h"
#include "../Simulation.h"

namespace Scheduling {
	void SJF::update(Simulation* sim) {
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