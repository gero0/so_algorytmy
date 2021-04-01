#pragma once
#include "IAlgorithm.h"
#include <algorithm>

namespace Scheduling {
	class SJF_Preemptive:
		public IAlgorithm
	{
		std::string getName() override {
			return "Preemptive SJF";
		}
		void update(Simulation* sim) override;
	};
}

