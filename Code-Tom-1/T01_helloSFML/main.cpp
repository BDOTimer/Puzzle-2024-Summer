#define SFML_STATIC
///----------------------------------------------------------------------------|
/// ...
/// отключить консольное окно: -mwindows
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

///----------------------------------------------------------------------------|
/// Run
///----------------------------------------------------------------------------:
struct  Run
{       Run() : window(sf::VideoMode(800, 600), "T01::Hello")

        {   hero.setPosition    ( 50, 100          );
            hero.setFillColor   (sf::Color(0,128,0));
            hero.setSize        ({700, 450}        );
            hero.setOutlineColor(sf::Color(222,0,0));
            hero.setOutlineThickness           (5.f);

            font.loadFromFile   ("c:/windows/fonts/georgia.ttf"); ///<-------!!!

            text.setFont                (font);
            text.setCharacterSize         (50);
            text.setStyle  (sf::Text::Regular);
            text.setFillColor(sf::Color::Blue);

            loop();
        }

    sf::RenderWindow window;
    sf::RectangleShape hero;
    sf::Font           font;
    sf::Text           text;

    void process_mouse(const sf::Vector2i& mouse_pos)
    {   std::string s("XY = {");
                    s += std::to_string(mouse_pos.x) + ", ";
                    s += std::to_string(mouse_pos.y) + "}" ;
        text.setString(s);
    }

    void loop()
    {
        sf::Vector2i mouse_pos;

        while (window.isOpen())
        {
            for (sf::Event event; window.pollEvent(event);)
            {
                if (event.type == sf::Event::Closed) window.close();

                ///------------------------------------|
                /// MouseMoved только здесь.           |
                ///------------------------------------:
                if (event.type == sf::Event::MouseMoved)
                {   mouse_pos  =  sf::Mouse::getPosition(window);
                    process_mouse(mouse_pos);
                }
            }

            window.clear   ();
            window.draw(hero);
            window.draw(text);
            window.display ();
        }
    }
};

///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int main()
{   std::unique_ptr<Run> run(new Run);
}
