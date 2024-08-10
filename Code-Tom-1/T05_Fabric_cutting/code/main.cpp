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

const sf::Vector2u WINSIZE{800, 600};
float deltaTime{0};

///----------------------------------------------------------------------------|
/// Интерфейс объектов рендера.
///----------------------------------------------------------------------------:
namespace Obj
{
    struct      IObject : sf::Drawable
    {   virtual~IObject(){}
        virtual void update   (                        ) = 0;
        virtual bool RPControl(std::string_view command,
                               std::vector<float>& arg ) = 0;
        virtual void input(const sf::Event&       event) = 0;

        std::string_view name;

    private:
    };
}

struct  Shaders_Test : Obj::IObject
{       Shaders_Test(const sf::RenderWindow& window, const sf::Font& font)
        {
            IEffect::setFont(font);

          //effects.push_back(new Effect_Test_01);
            effects.push_back(new Effect_Test_03(window.getSize()));

            for (std::size_t i = 0; i < effects.size(); ++i) effects[i]->load();
        }

    bool RPControl( std::string_view command,
                    std::vector<float>& arg ) { return true; }
    void input(const sf::Event&       event ) {}

    void update()
    {   for(auto p : effects) p->update(deltaTime, 0, 0);
    }

private:
    std::vector<IEffect*> effects;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto p : effects) target.draw(*p);
    }
};

///----------------------------------------------------------------------------|
/// Run
///----------------------------------------------------------------------------:
struct  Run
{       Run() : window(sf::VideoMode(WINSIZE.x,
                                     WINSIZE.y), "T04::Shader_Test-01")

        {
            effects = std::make_unique<Shaders_Test>(window, font);

            hero.setPosition    ( 50, 100          );
            hero.setFillColor   (sf::Color(0,128,0));
            hero.setSize        ({700, 450}        );
            hero.setOutlineColor(sf::Color(222,0,0));
            hero.setOutlineThickness           (5.f);

            font.loadFromFile   (getPath2Res("consola.ttf"));

            text.setFont                  (font);
            text.setCharacterSize           (18);
            text.setStyle    (sf::Text::Regular);
            text.setFillColor(sf::Color::Yellow);

            loop();
        }

    sf::RenderWindow window;
    sf::RectangleShape hero;
    sf::Font           font;
    sf::Text           text;

    ///----------------------------------|
    /// Effects definition.              |
    ///----------------------------------:
    std::unique_ptr<Obj::IObject> effects;

    sf::Clock         clock;

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

            if(!pause) effects->update();

            window.clear   ();
            window.draw(hero);
            window.draw(text);

            window.draw(*effects);

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
