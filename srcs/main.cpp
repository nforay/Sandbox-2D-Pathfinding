#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "../incs/TileMap.hpp"

int main()
{
	sf::RenderWindow window;
	sf::Font MyFont;
	if (!MyFont.loadFromFile("assets/fonts/visitor2.ttf"))
		return (1);
	window.create(sf::VideoMode(512, 512), "Tilemap");
	TileMap map(16, 16);
    if (!map.load("assets/images/tilemap.png", sf::Vector2u(32, 32)))
        return -1;
	window.clear(sf::Color(72, 215, 216, 255));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				//std::cout << "x: " << localPosition.x/32 << " y: " << localPosition.y/32 << std::endl;
				map.toggletile((localPosition.x/32), (localPosition.y/32));
			}
		}
		window.draw(map);
		for (unsigned int i = 0; i < 16; ++i)
			for (unsigned int j = 0; j < 16; ++j)
			{
				if (map.gettile(i, j))
				{
					sf::Text text;
					std::stringstream ss;
					ss << map.getref(i, j);
					text.setString(ss.str());
					text.setFont(MyFont);
					text.setFillColor(sf::Color::Blue);
					text.setCharacterSize(14);
					sf::FloatRect textRect = text.getLocalBounds();
					text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
					text.setPosition(sf::Vector2f(1.f * i * 32 + 16, 1.f * j * 32 + 16));
					window.draw(text);
				}
			}
		window.display();
	}

	return 0;
}
