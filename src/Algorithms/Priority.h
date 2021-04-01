#pragma once
#include "IAlgorithm.h"
#include <algorithm>

namespace Scheduling {
	class Priority :
		public IAlgorithm
	{
	public:
		Priority();
		Priority(float agingRate);
		std::string getName() override {
			return "Priorytetowy SJF";
		}
		void update(Simulation* sim) override;
	private:
		float agingRate;
	};
}


