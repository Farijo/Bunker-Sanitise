#include <SFML/Graphics.hpp>
#include "Stage.h"

int main()
{
    srand (time(NULL));

    Stage s(3);

    sf::RenderWindow window(sf::VideoMode(765, 765), "Bunker Sanitise");
    s.windowResize(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            if (event.type == sf::Event::Resized)
            {
              window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
              s.windowResize(event.size.width, event.size.height);
            }
        }

        window.clear();

        s.draw(window);

        window.display();
    }

    return 0;
}
