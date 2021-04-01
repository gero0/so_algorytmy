#pragma once
#include "IAlgorithm.h"
namespace Scheduling {
	class RoundRobin :
		public IAlgorithm
	{
	public:
		RoundRobin();
		RoundRobin(int quant);
		std::string getName() override {
			return "Round Robin";
		}
		void update(Simulation* sim) override;
	private:
		int timeQuant = 1;
		int currentProcessPosition = -1;
		int timeCounter = 0;
	};
}

