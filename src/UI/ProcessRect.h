#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#define PROCESSRECT_NO_PROCESS -1

namespace Scheduling {
	class ProcessRect : public sf::Drawable {
	public:
		int id;
		int fontSize = 10;
		int time = -1;
		sf::Font font;
		sf::Vector2f position;
		sf::Vector2f size = { 20,40 };

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			sf::RectangleShape rect;

			if (id != PROCESSRECT_NO_PROCESS) {
				rect.setFillColor(sf::Color::White);
			}
			else {
				rect.setFillColor(sf::Color::Black);
			}
			
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(1.0f);
			rect.setPosition(position);
			rect.setSize(size);

			target.draw(rect);

			if (time > 0) {
				sf::Text timetxt;
				timetxt.setString(std::to_string(time));
				timetxt.setCharacterSize(fontSize);
				timetxt.setFont(font);
				timetxt.setPosition(position.x + size.x - fontSize / 2, position.y + size.y + (fontSize / 2));
				timetxt.setFillColor(sf::Color::Black);

				target.draw(timetxt);
			}

			if (id != PROCESSRECT_NO_PROCESS) {
				sf::Text text;
				text.setString(std::to_string(id));
				text.setCharacterSize(fontSize);
				text.setFont(font);
				text.setPosition(position);
				text.setFillColor(sf::Color::Black);
				
				target.draw(text);
			}	
	
		}
	};
}


