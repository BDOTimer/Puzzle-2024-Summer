#define SFML_STATIC
///----------------------------------------------------------------------------|
/// ...
/// отключить консольное окно: -mwindows
///----------------------------------------------------------------------------:
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "effect_test.h"

///----------------------------------------------------------------------------|
/// Run
///----------------------------------------------------------------------------:
struct  Run
{       Run() : window(sf::VideoMode(800, 600), "T04::Shader_Test-01")

        {   hero.setPosition    ( 50, 100          );
            hero.setFillColor   (sf::Color(0,128,0));
            hero.setSize        ({700, 450}        );
            hero.setOutlineColor(sf::Color(222,0,0));
            hero.setOutlineThickness           (5.f);

            font.loadFromFile   ("c:/windows/fonts/georgia.ttf"); ///<-------!!!

            text.setFont                  (font);
            text.setCharacterSize           (18);
            text.setStyle    (sf::Text::Regular);
            text.setFillColor(sf::Color::Yellow);

            ///-------------------------------|
            /// Effects init.                 |
            ///-------------------------------:
            IEffect::setFont(font);

            effects.push_back(new Effect_Test_01);
            effects.push_back(new Effect_Test_02(window.getSize()));

            for (std::size_t i = 0; i < effects.size(); ++i) effects[i]->load();

            loop();
        }

    sf::RenderWindow window;
    sf::RectangleShape hero;
    sf::Font           font;
    sf::Text           text;

    ///-------------------------------|
    /// Effects definition.           |
    ///-------------------------------:
    std::vector<IEffect*> effects;
    std::size_t       current = 0;

    sf::Clock clock;

    void process_mouse(const sf::Vector2i& mouse_pos)
    {   std::string s("Mouse::XY = [");
                    s += std::to_string(mouse_pos.x) + ", ";
                    s += std::to_string(mouse_pos.y) + "]" ;
        text.setString(s);
    }

    void loop()
    {
        sf::Vector2i mouse_pos;

        bool pause = false;

        float deltaTime = 0;

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


                if (event.type == sf::Event::KeyPressed)
                {   ///------------------------------------------|
                    /// “ак как шейдер один, то не используетс¤. |
                    ///------------------------------------------:
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    case sf::Keyboard::Left:
                        /// ...
                        break;

                    case sf::Keyboard::Right:
                        /// ...
                        break;

                    case sf::Keyboard::Space:
                        pause = !pause;
                        break;

                    default:
                        break;
                }
                }
            }

            if(!pause)
            {
                effects[0]->update(deltaTime, 0, 0);
                effects[1]->update(deltaTime, 0, 0);
            }

            window.clear   ();
            window.draw(hero);
            window.draw(text);

            window.draw(*effects[1]);
            window.draw(*effects[0]);

            window.display ();

            deltaTime = clock.restart().asSeconds();
        }
    }
};

///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int main()
{   std::unique_ptr<Run> run(new Run); return 0;
}
