#pragma once
#include "IAlgorithm.h"
#include <string>

namespace Scheduling {
	class Simulation;
	
	class FCFS :
		public IAlgorithm
	{
	public:
		std::string getName() override {
			return "FCFS";
		}
		void update(Simulation* sim) override;
	};
}


