/// C++17
///----------------------------------------------------------------------------|
/// Shaders.
///----------------------------------------------------------------------------:
#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#include <cassert>
#include <string>

#include "effect_test.h"

#include <iostream>
///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int xmain()
{   std::cout << "Made by MSYS2 " << __VERSION__ << '\n';;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    ///----------|
    /// Font.    |
    ///----------:
    sf::Font font;
    if (!font.loadFromFile("C:/windows/fonts/georgia.ttf"))
        return EXIT_FAILURE;
    IEffect::setFont(font);

    ///----------|
    /// Effects. |
    ///----------:
    std::vector<IEffect*> effects;
                          effects.push_back(new Effect_Test_01);

    std::size_t current = 0;

    for (std::size_t i = 0; i < effects.size(); ++i) effects[i]->load();

    ///-----------------------|
    /// Messages background.  |
    ///-----------------------:
    sf::Texture textBackgroundTexture;
    if (!textBackgroundTexture.loadFromFile("myres/text-background.png"))
        return EXIT_FAILURE;

    sf::Sprite textBackground            (textBackgroundTexture);
               textBackground.setPosition(0, 520);
               textBackground.setColor   (sf::Color(255, 255, 255, 200));

    ///---------------|
    /// Description.  |
    ///---------------:
    sf::Text description
    ("Current effect: " + effects[current]->getName(), font, 20);
    description.setPosition(10, 530);
    description.setFillColor(sf::Color(80, 80, 80));

    ///---------------|
    /// Instructions. |
    ///---------------:
    sf::Text instructions
    ("Press left and right arrows to change the current shader", font, 20);
    instructions.setPosition(280, 555);
    instructions.setFillColor(sf::Color(80, 120, 250));

    sf::Clock clock;
    ///---------------------|
    /// Start the game loop.|
    ///---------------------:
    while (window.isOpen())
    {   sf::Event event;
        while (window.pollEvent(event))
        {   // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {   ///------------------------------------------|
                /// “ак как шейдер один, то не используетс¤. |
                ///------------------------------------------:
                switch (event.key.code)
                {       // Escape key: exit
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    // Left arrow key: previous shader
                    case sf::Keyboard::Left:
                        if (current == 0)
                            current = effects.size() - 1;
                        else
                            current--;
                        description.setString
                        ("Current effect: " + effects[current]->getName());
                        break;

                    // Right arrow key: next shader
                    case sf::Keyboard::Right:
                        if (current == effects.size() - 1)
                            current = 0;
                        else
                            current++;
                        description.setString
                        ("Current effect: " + effects[current]->getName());
                        break;

                    default:
                        break;
                }
            }
        }

        ///----------------------------|
        /// Update the current example.|
        ///--------------------------------------------------------------------:
        float x = static_cast<float>
                  (sf::Mouse::getPosition(window).x) / window.getSize().x;
        float y = static_cast<float>
                  (sf::Mouse::getPosition(window).y) / window.getSize().y;
        effects[current]->update(clock.getElapsedTime().asSeconds(), x, y);

        ///----------------------------|
        /// Draw.                      |
        ///--------------------------------------------------------------------:
        window.clear(sf::Color(45, 90, 120));

        window.draw(*effects[current]);

        window.draw(textBackground);
        window.draw(instructions);
        window.draw(description);

        window.display();///<--------------------------------------------------.
    }

    for (std::size_t i = 0; i < effects.size(); ++i)
        delete effects[i];

    return EXIT_SUCCESS;
}
