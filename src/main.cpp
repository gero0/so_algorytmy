#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "Simulation.h"
#include "UI/UI_SFML.h"
#include "Algorithms/FCFS.h"
#include "Algorithms/RoundRobin.h"
#include "Algorithms/SJF.h"
#include "Algorithms/SJF_Preemptive.h"
#include "Algorithms/Priority.h"

int main() {
	std::vector<Scheduling::Simulation> sims;
	std::string filename;
	
	std::cout << "Podaj nazwe pliku z danymi: ";
	std::cin >> filename;

	//catch any exception that may occur while initializing the simulation
	std::unique_ptr<Scheduling::IAlgorithm> fcfs;
	std::unique_ptr<Scheduling::IAlgorithm> sjf;
	std::unique_ptr<Scheduling::IAlgorithm> rr;
	std::unique_ptr<Scheduling::IAlgorithm> pr;

	try {
		fcfs = std::make_unique<Scheduling::FCFS>();
		sjf = std::make_unique<Scheduling::SJF>();
		rr = std::make_unique<Scheduling::RoundRobin>();
		pr = std::make_unique<Scheduling::Priority>();

	
		Scheduling::Simulation fcfsSim(fcfs.get());
		Scheduling::Simulation sjfSim(sjf.get());
		Scheduling::Simulation rrSim(rr.get());
		Scheduling::Simulation prSim(pr.get());
	
		std::cout << "Ladowanie...\n";

		fcfsSim.loadFromFile(filename);
		sjfSim.loadFromFile(filename);
		rrSim.loadFromFile(filename);
		prSim.loadFromFile(filename);
	
		std::cout << "OK\n";

		sims.push_back(fcfsSim);
		sims.push_back(sjfSim);
		sims.push_back(rrSim);
		sims.push_back(prSim);
	}
	catch (const std::exception& e) {
		std::cout << "Nie mozna uruchomic programu.\nBlad: " + std::string(e.what());
		return -1;
	}

	Scheduling::UI_SFML ui(sims);
	ui.display();
}