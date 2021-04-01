#pragma once
#include <string>

namespace Scheduling {
	class Simulation;

	class IAlgorithm
	{
	public:
		virtual std::string getName() = 0;
		virtual void update(Simulation* sim) = 0;
	};
}

