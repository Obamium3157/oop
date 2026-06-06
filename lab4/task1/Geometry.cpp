#include "CCanvas.h"
#include "GeometryHandler.h"

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    GeometryHandler handler;
    handler.ReadShapes(std::cin);
    handler.PrintResults(std::cout);

    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Geometry");
    window.setFramerateLimit(60);
    CCanvas canvas(window);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        handler.Draw(canvas);
        window.display();
    }

    return 0;
}
