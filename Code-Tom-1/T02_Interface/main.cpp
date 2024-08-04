#define SFML_STATIC
///----------------------------------------------------------------------------|
/// Tutor-02: Interface
///     - для объектов с разной логикой удобоваримых для рендера.
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>

#include <SFML/Graphics.hpp>

///----------------------------------------------------------------------------|
/// Интерфейс объектов рендера.
///----------------------------------------------------------------------------:
namespace Obj
{
    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
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

///----------------------------------------------------------------------------|
/// Переключатель сцен.
///----------------------------------------------------------------------------:
struct ManagerScenes
{
    enum eSCENES
    {   E_LOGO,
        E_GAME,
        E_NONE
    };

    void bindScene(eSCENES n, Obj::IObject* o)
    {   SCENES   = n;
        (*scene) = o;
    }

    eSCENES getScene() const { return SCENES; }

    void setPScene(Obj::IObject** pscn) { scene = pscn; }

private:
    eSCENES SCENES{E_LOGO};

    Obj::IObject** scene = nullptr;
};


///----------------------------------------------------------------------------|
/// Конфиг.
///----------------------------------------------------------------------------:
///---------------------|
/// Общая часть.        |
///---------------------:
struct  Config
{
    sf::Vector2i winsize{700 * 1366 / 768, 700};

    sf::Font font;
};

///---------------------|
/// Игровая часть.      |
///---------------------:
struct  ConfigGame : Config
{
    ManagerScenes managerScenes;

    std::vector<std::unique_ptr<Obj::IObject>> objs;

    void init()
    {
        font.loadFromFile( "c:/windows/fonts/tahoma.ttf");

        ///------------------------------|
        /// Получаем набор объектов.     |
        ///------------------------------:
        extern std::vector<std::unique_ptr<Obj::IObject>> createObjs();
        objs =                                            createObjs();
    }

    static inline ConfigGame* p = nullptr;
};


///----------------------------------------------------------------------------|
/// Тестовый графический примитив, который можно отобразить на экране.
///----------------------------------------------------------------------------:
struct  TestA : sf::RectangleShape
{       TestA(sf::Vector2f sz, sf::Vector2f pos, sf::Color color)
        {
            setSize        (sz);

            const auto x = ConfigGame::p->winsize.x - getSize().x;
            const auto y = ConfigGame::p->winsize.y - getSize().y;

            setPosition    ( x/2 + pos.x,
                             y/2 + pos.y);
            setFillColor   (color);
            setOutlineColor(sf::Color(222,0,0));
            setOutlineThickness           (2.f);

            text.setFont       (ConfigGame::p->font);
            text.setCharacterSize               (50);
            text.setStyle        (sf::Text::Regular);
            text.setFillColor     (sf::Color::Green);
            text.setPosition                ({0,30});
        }

        sf::Text text;
};


///----------------------------------------------------------------------------|
/// SceneLogo.
///----------------------------------------------------------------------------:
struct  SceneLogo : Obj::IObject, TestA
{       SceneLogo() : TestA({650,400}, {0, 0}, {0,128,64})
        {
            name = "SceneLogo";
            text.setString(std::string(name));
        }

    void update   (                        ) {}
    bool RPControl(std::string_view command,
                   std::vector<float>& arg ) { return true; };

    void input    (const sf::Event&   event)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            ConfigGame::p->managerScenes.bindScene(
                ManagerScenes::E_GAME,
                ConfigGame::p->objs[ManagerScenes::E_GAME].get()
            );
        }
    }

private:
    ///-----------------|
    /// На рендер.      |
    ///-----------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {
        target.draw(TestA(*this), states);
        target.draw(TestA::text , states);
    }
};


///----------------------------------------------------------------------------|
/// SceneGame.
///----------------------------------------------------------------------------:
struct  SceneGame : Obj::IObject, TestA
{       SceneGame() : TestA({300,500}, {-200, -50}, {0,0,255})
        {
            name = "SceneGame";
            text.setString(std::string(name));
        }

    void update   (                        ) {}
    bool RPControl(std::string_view command,
                   std::vector<float>& arg ) { return true; };

    void input    (const sf::Event&   event)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    //  if (event.key.code == sf::Keyboard::Escape)
        {
            ConfigGame::p->managerScenes.bindScene(
                ManagerScenes::E_LOGO,
                ConfigGame::p->objs[ManagerScenes::E_LOGO].get()
            );
        }
    }

private:
    ///-----------------|
    /// На рендер.      |
    ///-----------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {
        target.draw(TestA(*this), states);
        target.draw(TestA::text , states);
    }
};


///----------------------------------------------------------------------------|
/// Фабрика-функция создаёт нужный набор объектов.
///----------------------------------------------------------------------------:
std::vector    <std::unique_ptr<Obj::IObject>> createObjs()
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
            "T02::Interface")

        {
            window.setVerticalSyncEnabled(true);
        //  window.setFramerateLimit     (50  );

            text.setFont       (ConfigGame::p->font);
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
    /// Графич. примитив.  |
    ///--------------------:
    sf::Text           text;

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

            window.clear     ();

            ///----------------|
            /// Отрисовка...   |
            ///----------------:
            window.draw( text );
            window.draw(*scene);

            ///----------------|
            /// На дисплей...  |
            ///----------------:
            window.display   ();
        }
    }
};


///----------------------------------------------------------------------------|
/// Programm.
///----------------------------------------------------------------------------:
struct  Programm
{       Programm()
        {
            ///------------------------------|
            /// Инициализация конфига.       |
            ///------------------------------:
            ConfigGame cfg; ConfigGame::p = &cfg; cfg.init();

            ///------------------------------|
            /// Запуск теста...              |
            ///------------------------------:
            std::unique_ptr<RenderLoop> testmain(new RenderLoop);
                                        testmain->run();
        }
};


///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int main()
{
    Programm programm;
}
