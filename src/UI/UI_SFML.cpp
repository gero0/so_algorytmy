#include "UI_SFML.h"
namespace Scheduling {
	UI_SFML::UI_SFML(std::vector<Simulation> sims) :
		window(sf::VideoMode(1920, 600), "Scheduling")
	{
		font.loadFromFile("arial.ttf");
		window.setFramerateLimit(60);
		this->sims = sims;
		simulationThread = std::make_unique<std::thread>(&UI_SFML::simulationProc, this);

		for (auto sim : sims)
		{
			rects.push_back(std::vector<ProcessRect>());
		}
	}

	void UI_SFML::update() {
		int i = 0;

		currentStep = sims[0].currentStep;

		if (currentStep < 0)
			return;

		drawMutex.lock();

		for (auto& sim : sims) {
			auto& rectangles = rects[i];
			if (sim.currentProcess.has_value()) {
				if (rectangles.empty() || (sim.currentProcess.value().id != rectangles.back().id && !simFinished)) {

					ProcessRect rect;
					rect.font = font;
					rect.fontSize = fontSize;
					rect.id = sim.currentProcess.value().id;
					if (rectangles.empty()) {
						rect.position = { diagramPosition.x, diagramPosition.y + i * diagramOffset };
					}
					else {
						rect.position = {
							rectangles.back().position.x + rectangles.back().size.x,
							diagramPosition.y + i * diagramOffset
						};
					}

					if (!rectangles.empty()) {
						rectangles.back().time = currentStep-1;
					}
					rectangles.push_back(rect);
				}
				else{
					rectangles.back().size.x += 20;
				}
			}
			else if(currentStep > 0 && !simFinished) {
				if (rects[i].back().id == PROCESSRECT_NO_PROCESS) {
					rects[i].back().size.x += 20;
				}
				else {
					ProcessRect rect;
					rect.id = PROCESSRECT_NO_PROCESS;
					rect.position = {
						rectangles.back().position.x + rectangles.back().size.x,
						diagramPosition.y + i * diagramOffset
					};
				
					rects[i].back().time = currentStep - 1;
					rects[i].push_back(rect);
				}
				
			}
			i++;
		}

		drawMutex.unlock();
	}

	void UI_SFML::advanceSimulation() {
		for (auto& sim : sims) {
			sim.step();
		}
		
		//can't be in 1 loop
		simFinished = true;
		for (auto& sim : sims) {
			if (!sim.hasFinished()) {
				simFinished = false;
			}
		}

		update();	
	}

	void UI_SFML::simulationProc() {
		while (!simFinished) {
			if (!manualStep)
				advanceSimulation();
			if(!simFinished)
				std::this_thread::sleep_for(std::chrono::milliseconds(timeScale));
		}	
		//SIMULATION FINISHED, Calculate avg waiting times and write reports
		for (auto& sim : sims) {
			sim.writeResultsToFile("./results/"+sim.getAlgorithmName()+"_report.csv");
		}
		timesReady = true;
	}

	void UI_SFML::display() {
		windowLoop();
	}

	void UI_SFML::addProcess() {
		Process process;
		std::cout << "Nowy Proces:\n";

		std::cout << "Id: ";
		std::cin >> process.id;
		if (process.id < 0 || std::cin.fail()) {
			std::cout << "Id nie moze byc mniejsze niz 0!\n";
			std::cout << "Nie udalo sie dodac procesu\n";
			//reset input buffer to get rid of possible garbage data
			std::cin.clear();
			return;
		}

		std::cout << "Czas przybycia (obecny czas: " << currentStep << "): ";
		std::cin >> process.arrivalTime;
		if (process.arrivalTime < currentStep || process.arrivalTime < 0 || std::cin.fail()) {
			std::cout << "Czas przybycia nie mo¿e byæ mniejszy ni¿ obecny!\n";
			std::cout << "Nie udalo sie dodac procesu\n";
			//reset input buffer to get rid of possible garbage data
			std::cin.clear();
			return;
		}

		std::cout << "Czas wykonania: ";
		std::cin >> process.burstTime;
		if (process.burstTime < 1 || std::cin.fail()) {
			std::cout << "Czas wykonania nie moze byc mniejszy niz 1!\n";
			std::cout << "Nie udalo sie dodac procesu\n";
			//reset input buffer to get rid of possible garbage data
			std::cin.clear();
			return;
		}
		
		std::cout << "Priorytet: ";
		std::cin >> process.priority;
		if (process.priority < 0 || std::cin.fail()) {
			std::cout << "Priorytet nie moze byc mniejszy od 0!\n";
			std::cout << "Nie udalo sie dodac procesu\n";
			//reset input buffer to get rid of possible garbage data
			std::cin.clear();
			return;
		}

		for (auto& sim : sims) {
			sim.newProc.push_back(process);
		}
		
		std::cout << "Dodano proces!\n";
	}

	void UI_SFML::windowLoop() {
		while (window.isOpen()) {
			//Handle Events
			sf::Event event;
			while (window.pollEvent(event))
			{
				// Request for closing the window
				if (event.type == sf::Event::Closed) {
					if (!simFinished) {
						//forcing sim thread to stop
						simulationThread->~thread();
					}
					else {
						simulationThread->join();
					}
						
					window.close();
				}
				else if (event.type == sf::Event::KeyPressed) {
					if(event.key.code == sf::Keyboard::Enter) {
						manualStep = false;
					}
					else if(event.key.code == sf::Keyboard::Space){
						if (manualStep) {
							//If manual mode is active, advance simulation
							if(!simFinished)
								advanceSimulation();
						}
						else {
							//otherwise, activate manual mode
							manualStep = true;
						}
					}
					else if(event.key.code == sf::Keyboard::A){
						manualStep = true;
						addProcess();
					}
					else if (event.key.code == sf::Keyboard::Add) {
						timeScale += 100;
					}
					else if (event.key.code == sf::Keyboard::Subtract) {
						timeScale -= 100;
						if (timeScale < 0) {
							timeScale = 0;
						}
					}
				}
			}

			//draw frame
			drawObjects();
		}
	}

	void UI_SFML::drawText(std::string txt, sf::Vector2f position, sf::Color color) {
		sf::Text text;
		text.setString(txt);
		text.setCharacterSize(fontSize);
		text.setFont(font);
		text.setPosition(position);
		text.setFillColor(color);

		window.draw(text);
	}

	void UI_SFML::drawObjects() {
		window.clear(sf::Color::White);
		//draw text
		drawText("Czas (ms): " + std::to_string(currentStep), currentStepPosition);
		drawText("Skala czasu: " + std::to_string(timeScale), timeScalePosition);
		
		if (!manualStep) {
			drawText("AUTO", manualPosition);
		}

		drawMutex.lock();

		int i = 0;
		for (auto sim : sims) {
			drawText(sim.getAlgorithmName(), { diagramPosition.x, diagramPosition.y - 3 * fontSize + i * diagramOffset });
			
			if (!rects[i].empty()) {
				auto rectangles = rects[i];
				auto& r = rectangles.at(0);
				drawText("0", r.position + sf::Vector2f(0, r.size.y + fontSize / 2));

				for (auto& rectangle : rectangles) {
					window.draw(rectangle);
				}
			}
			i++;
		}

		if (simFinished) {
			drawText("Symulacja zakoñczona.", simFinishedPosition);
			int i = 0;
			if (timesReady) {
				for (auto sim : sims) {
					drawText("Sredni czas oczekiwania: " + std::to_string(sim.avgWaitingTime), 
						{ diagramPosition.x, diagramPosition.y + i * diagramOffset - 2*fontSize });
					
					if (!rects[i].empty()) {
						drawText(std::to_string(currentStep),
							rects[i].back().position + rects[i].back().size + sf::Vector2f(-fontSize / 2, fontSize / 2));
					}
					i++;
				}
			}
		}

		window.display();

		drawMutex.unlock();
	}
}