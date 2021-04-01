#include "Simulation.h"
#include "UI/IUI.h"

namespace Scheduling {
	Simulation::Simulation(IAlgorithm* algorithm) {
		try {
			this->algorithm = algorithm;
		}
		catch (const std::exception& e) {
			std::cout << "Wystapil blad! \nBlad: " + std::string(e.what());
			exit(-1);
		}
	}

	void Simulation::step() {
		if (!hasFinished()) {
			algorithm->update(this);
		}
		if (!hasFinished()) {
			currentStep++;
		}
	}

	bool Simulation::hasFinished() {
		return (newProc.empty() && readyProc.empty() && !currentProcess.has_value());
	}

	std::string Simulation::getAlgorithmName() {
		return algorithm->getName();
	}

	void Simulation::loadFromFile(std::string filepath) {
		std::fstream file(filepath);
		std::string line;

		if (!file.good())
			throw std::runtime_error("Nie mozna otworzyc pliku. Plik moze nie istniec");

		while (std::getline(file, line)) {
			Process process;
			std::stringstream ss(line);
			std::string substr;

			std::getline(ss, substr, ',');
			if (ss.fail()) {
				throw std::runtime_error("Nieprawidlowy format pliku!");
			}			
			process.id = std::stoi(substr);
			if (process.id < 0) {
				throw std::runtime_error("Nieprawidlowe dane: ID < 0");
			}

			std::getline(ss, substr, ',');
			if (ss.fail()) {
				throw std::runtime_error("Nieprawidlowy format pliku!");
			}
			process.arrivalTime = std::stoi(substr);
			if (process.arrivalTime < 0) {
				throw std::runtime_error("Nieprawidlowe dane: czas przybycia < 0");
			}

			std::getline(ss, substr, ',');
			if (ss.fail()) {
				throw std::runtime_error("Nieprawidlowy format pliku!");
			}
			process.burstTime = std::stoi(substr);
			if (process.burstTime < 1) {
				throw std::runtime_error("Nieprawidlowe dane: czas wykonywania < 1");
			}

			std::getline(ss, substr, ',');
			if (ss.fail()) {
				throw std::runtime_error("Nieprawidlowy format pliku!");
			}
			process.priority = std::stof(substr);
			if (process.priority < 0) {
				throw std::runtime_error("Nieprawidlowe dane: Priorytet < 0");
			}

			newProc.push_back(process);
		}
	}

	void Simulation::writeResultsToFile(std::string filepath) {
		std::ofstream file(filepath);

		for (auto proc : finishedProc) {
			avgWaitingTime += proc.waitingTime;
		}
		avgWaitingTime /= finishedProc.size();

		file << "Average waiting time: " << avgWaitingTime << "\n";
		file << "ID;Arrival_Time;Burst_time;Completion_time;Waiting_Time;Priority\n";
		for (Process proc : finishedProc) {
			file << std::to_string(proc.id) << ";" << std::to_string(proc.arrivalTime) << ";"
				<< std::to_string(proc.burstTime) << ";" << std::to_string(proc.completionTime) << ";"
				<< std::to_string(proc.waitingTime) << ";" << std::to_string(proc.priority) << "\n";
		}
	}
}