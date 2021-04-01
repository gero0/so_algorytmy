#pragma once

namespace Scheduling {
	class Process
	{
	public:
		int id = 0;
		int arrivalTime = 0;
		int burstTime = 0;
		//used only by priority scheduling algorithms
		float priority = 0;

		int executionTime = 0;
		int completionTime = 0;
		int waitingTime = 0;
	};
}


