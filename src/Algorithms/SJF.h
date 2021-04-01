#pragma once
#include "IAlgorithm.h"
#include <algorithm>

namespace Scheduling {
	class SJF :
		public IAlgorithm
	{
		std::string getName() override {
			return "Niewywlaszczajacy SJF";
		}
		void update(Simulation* sim) override;
	};
}


