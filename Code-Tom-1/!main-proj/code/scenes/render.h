#ifndef RENDER_H
#define RENDER_H
///----------------------------------------------------------------------------|
/// "render.h"
///----------------------------------------------------------------------------:

#include "config.h"
#include "logo\logo.h"
#include "game\game.h"
#include "fps.h"


///----------------------------------------------------------------------------|
/// Фабрика-функция создаёт нужный набор объектов.
///----------------------------------------------------------------------------:
inline std::vector    <std::unique_ptr<Obj::IObject>> createObjs()
{   std::vector<std::unique_ptr<Obj::IObject>> v;
    v.push_back(std::make_unique<SceneLogo>());
    v.push_back(std::make_unique<SceneGame>());
    return v;
}


///----------------------------------------------------------------------------|
/// RenderLoop
///----------------------------------------------------------------------------:
struct  RenderLoop
{       RenderLoop() : window(sf::VideoMode(ConfigGame::p->winsize.x,
                                            ConfigGame::p->winsize.y),
                                            "T03::TexturesHolder"   ),
                       fps(cam_ui)
        {
            window.setVerticalSyncEnabled(true);
        //  window.setFramerateLimit     (50  );

            ///----------------------|
            /// Камеры вида.         |
            ///----------------------:
            cam_ui    = window.getView();
            cam_world = cam_ui;
            cam_world.setCenter({0,0});

            ConfigGame::p->view[0] = &cam_world;
            ConfigGame::p->view[1] = &cam_ui   ;

            text.setFont     (ConfigRender::p->font);
            text.setCharacterSize               (18);
            text.setStyle        (sf::Text::Regular);
            text.setFillColor     (sf::Color::White);

            ConfigGame::p->managerScenes.setPScene(&scene);

            ConfigGame::p->managerScenes.bindScene(
                ManagerScenes::E_LOGO,
                ConfigGame::p->objs[ManagerScenes::E_LOGO].get()
            );
        }

    void run() { loop(); };

private:
    ///--------------------|
    /// Главное окно.      |
    ///--------------------:
    sf::RenderWindow window;

    ///--------------------|
    /// Камеры вида.       |
    ///--------------------:
    sf::View      cam_world;
    sf::View      cam_ui   ;

    ///--------------------|
    /// Графич. примитив.  |
    ///--------------------:
    sf::Text           text;

    sf::Clock         clock;
    Fps                 fps;

    void process_mouse(const sf::Vector2i& mouse_pos)
    {   std::string s("mouse::XY = [");
                    s += std::to_string(mouse_pos.x) + ", ";
                    s += std::to_string(mouse_pos.y) + "]" ;
        text.setString(s);
    }

    ///--------------------|
    /// Текущая сцена.     |
    ///--------------------:
    Obj::IObject* scene = nullptr;

    ///--------------------|
    /// Главный цикл.      |
    ///--------------------:
    void loop()
    {
        sf::Vector2i mouse_pos;

        while (window.isOpen())
        {
            ///---------------------|
            /// Обработка сообщений.|
            ///---------------------:
            for (sf::Event event; window.pollEvent(event);)
            {
                if (event.type == sf::Event::Closed) window.close();

                if (event.type == sf::Event::KeyPressed)
                {   scene->input(event);
                }

                ///------------------------------------|
                /// MouseMoved только здесь.           |
                ///------------------------------------:
                if (event.type == sf::Event::MouseMoved)
                {   mouse_pos  =  sf::Mouse::getPosition(window);
                    process_mouse(mouse_pos);
                }
            }

            fps.update();

            window.clear();

            ///----------------------|
            /// cam_world.           |
            ///----------------------:
            window.setView(cam_world);

            ///----------------|
            /// Отрисовка...   |
            ///----------------:
            window.draw(*scene);

            ///----------------------|
            /// cam_ui.              |
            ///----------------------:
            window.setView   (cam_ui);
            window.draw        (text);
            window.draw         (fps);

            ///----------------|
            /// На дисплей...  |
            ///----------------:
            window.display   ();

            Config::deltaTime = clock.restart().asSeconds();
        }
    }
};


#endif // RENDER_H
