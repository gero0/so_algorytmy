#pragma once
#include "IUI.h"
#include "ProcessRect.h"
#include "SFML/Graphics.hpp"
#include "../Simulation.h"
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

namespace Scheduling {
	class UI_SFML :
		public IUI
	{
	public:
		UI_SFML(std::vector<Simulation> sims);
		void display();
	
	private:
		std::vector<Simulation> sims;
		std::vector<std::vector<ProcessRect>> rects;

		std::unique_ptr<std::thread> simulationThread;
		std::mutex drawMutex;

		int timeScale = 500;
		int currentStep = 0;

		//Manual step allows the user to move simulation step-by-step by pressing a key on keyboard
		//When this is set false, the simulation progresses automatically
		bool manualStep = true;
		bool simFinished = false;
		bool timesReady = false;

		void windowLoop();
		void update();
		void drawProcess(Process process, sf::Vector2f position, sf::Color color = sf::Color::Black,
			bool drawArrival = false, bool drawBurst = false, bool drawCompletion = false, bool drawPriority = false
		);
		void drawText(std::string txt, sf::Vector2f position, sf::Color color = sf::Color::Black);
		void drawObjects();
		void advanceSimulation();
		void simulationProc();
		void addProcess();

		sf::Font font;
		sf::RenderWindow window;

		const sf::Color currentColor {104,237,140,255};
		const sf::Color newColor{ 69,204,204,255 };
		const sf::Color readyColor{ 242,229,114,255 };
		const sf::Color finishedColor{ 222,221,209,255 };

		const int fontSize { 12 };
		
		const sf::Vector2f diagramPosition{ 30,110 };
		const int diagramOffset = 100;

		const sf::Vector2f manualPosition { 230,0 };
		const sf::Vector2f currentStepPosition { 0,0 };
		const sf::Vector2f simFinishedPosition { 0,10 };
		
		const sf::Vector2f timeScalePosition { 130,0 };
		const sf::Vector2f processBoxDimensions { 120, 80 };
		const sf::Vector2f newPosition { 20,150 };
		const sf::Vector2f readyPosition { 220,150 };
		const sf::Vector2f finishedPosition { 420,150 };
		const sf::Vector2f currentPosition { 20,50 };

		const sf::Vector2f avgWaitTimePosition{ 0,20 };
	};
}

