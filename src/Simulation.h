#pragma once
#include <vector>
#include <optional>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Process.h"
#include "Algorithms/IAlgorithm.h"

namespace Scheduling {
	class Simulation
	{
	public:
		Simulation(IAlgorithm* algorithm);

		//Current step of the simulation
		int currentStep = 0;
		//Loaded processes always start here
		std::vector<Process> newProc;
		//Processes which had arrived are stored here
		std::vector<Process> readyProc;
		//Finished processes
		std::vector<Process> finishedProc;
		//Currently executed process, might be empty, this is why we use optional
		std::optional<Process> currentProcess;

		void step();
		bool hasFinished();
		std::string getAlgorithmName();
		void loadFromFile(std::string filepath);
		void writeResultsToFile(std::string filepath);
		float avgWaitingTime = 0.f;

	private:
		//Pointer to algorithm that does the scheduling
		IAlgorithm* algorithm;
	};
}

