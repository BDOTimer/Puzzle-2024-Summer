#define SFML_STATIC
///----------------------------------------------------------------------------|
/// Tutor-03: Тестуры, Спрайты, Камеры, FPS.

/// Тест состоит из 3х компонент:
///     1. Ресурсы с холдерами текстур и спрайтов.
///     2. Рендер-спрайт.
///     3. Класс главного цикла.

/// 1. Рисуем жпег файл в R-текстуру единожды.
/// 2. Выводим R-текстуру на экран в главном цикле.

/// - две камеры: для гуи и для мира(на старте начало координат в центре экрана)
/// - автоматизация холдера текстур.
/// - быстрый способ получить спрайт с нужной текстурой.
/// - целевая текстура загружается из файла только один раз.

///----------------------------------------------------------------------------:
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <list>
#include <map>

#include <SFML/Graphics.hpp>

#define  l(v)  std::wcout << #v << " = " << (v) << "\n";

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

    sf::View* view[2];

    sf::View& getView_World(){ return *view[0]; }
    sf::View& getView_GUI  (){ return *view[1]; }

    static inline double deltaTime = 0.0;

    static inline std::string path2res{"../../../../Resources/"};

    static inline const char* filename[]
    {   "01.jpg",
        "02.jpg",
        "photo_2024-08-04_20-24-52.jpg"
    };

    static std::string getFileName(unsigned n)
    {   return Config::path2res + filename[n];
    }
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
/// Расштопаные примитивы.
/// (возмжность хранить дополнительные данные внутри себя)
///----------------------------------------------------------------------------:
struct Texture : sf::Texture
{
    bool load(const char* nm)
    {   name = nm;
        return loadFromFile (nm);
    }

    const char* name;

    std::wstring info() const
    {
        std::wstringstream s;
                           s << "Texture::"          << name
                             << "\n  getSize().x = " << getSize().x
                             << "\n  getSize().y = " << getSize().y
                             << "\n\n";
        return             s.str();
    }
};

struct  Sprite : sf::Sprite
{
    sf::Vector2u getSize() const { return getTexture()->getSize(); }
};


///----------------------------------------------------------------------------|
/// Общие ресурсы.
///----------------------------------------------------------------------------:
///--------------------|
/// Базовая часть.     |
///--------------------:
struct  Resources
{       Resources() : sprites(textures)
        {
        }

protected:
    struct  TexturesHolder
    {
        const Texture& getTexture(const char* name)
        {   if(auto p = m.find(name); p != m.end())
            {   return *p->second;
            }

            if(a.push_back(Texture()); a.back().load(name))
            {   m[name] = &a.back();
                return     a.back();
            }

            return t;
        }

        std::list<Texture>                   a;
        std::map<std::string_view, Texture*> m;
        Texture                              t;

    }textures;

    struct  SpriteHolder
    {       SpriteHolder(TexturesHolder& h) : TH(h) {}

        Sprite& getSprite(const char* name)
        {
            const Texture& t = TH.getTexture(name);
            a.push_back(Sprite ());
            a.back().setTexture(t);

            ///-----------------------------------|
            /// Узнаем размеры целевой жпега.     |
            ///-----------------------------------:
            auto szt = t.getSize();

            l(szt.x)
            l(szt.y)

            ///-----------------------------------|
            /// Устанавливаем центр в середину.   |
            ///-----------------------------------:
            a.back().setOrigin ({(float)szt.x/2,
                                 (float)szt.y/2});

            return a.back();
        }

        std::list<Sprite> a;

        TexturesHolder& TH;
    }sprites;
};

///--------------------|
/// Интерфейсная часть.|
///--------------------:
struct  IResources : Resources
{
    const Texture& getTexture(const char* name)
    {   return textures.getTexture (name);
    }

    Sprite* getSprite(const char* name)
    {   return &sprites.getSprite (name);
    }
}ress;

///----------------------------------------------------------------------------|
/// RenderSprite
/// Это спрайт, у которого внутри R-текстура(sf::RenderTexture)
///  - размеры жпега натягиваются на размеры R-текстуры.
///  - R-спрайт рисуется по центру экрана.
///----------------------------------------------------------------------------:
struct  RenderSprite : Sprite
{
    bool loadTexture(const char* name)
    {
        ///-----------------------------------|
        /// Создаём R-текстуру размера sz.    |
        ///-----------------------------------:
        if (!rt.create(szr.x, szr.y))
        {
            /// TODO::Error
            return false;
        }

        ///-----------------------------------|
        /// Дебаг инфы о жпег-картинке.       |
        ///-----------------------------------:
        l(ress.getTexture(name).info())

        ///-----------------------------------|
        /// Получаем спрайт с текстурой.      |
        ///-----------------------------------:
        sp = ress.getSprite(name);

        ///-----------------------------------|
        /// Узнаем размеры целевой жпега.     |
        ///-----------------------------------:
        szt = ress.getTexture(name).getSize();

        ///-----------------------------------|
        /// Камера внутри R-текстуры.         |
        ///-----------------------------------:
        view.setCenter(0,0);
        view.setSize  (sf::Vector2f(szt.x, szt.y));

        ///-----------------------------------|
        /// Рисуем.                           |
        ///-----------------------------------:
        rt.setView (view);
        rt.draw     (*sp);
        rt.display     ();

        ///-----------------------------------|
        /// Одеваем R-текстуру на спрайт.     |
        ///-----------------------------------:
        setTexture(rt.getTexture());
        setOrigin ({(float)szr.x/2,
                    (float)szr.y/2});
        return true;
    }

private:
    sf::RenderTexture rt;
    Sprite*           sp;
    sf::View        view;

    const sf::Vector2i szr{ConfigGame::p->winsize.x,
                           ConfigGame::p->winsize.y}; /// Размеры R-текстуры.
          sf::Vector2u szt;                          /// Размеры жпег-картинки.
};


///----------------------------------------------------------------------------|
/// Тестовый графический примитив, который можно отобразить на экране.
///----------------------------------------------------------------------------:
struct  TestA : sf::RectangleShape
{       TestA(sf::Vector2f sz, sf::Vector2f pos, sf::Color color)
        {
            setSize        (sz);

            const auto x = getSize().x / 2;
            const auto y = getSize().y / 2;

            setOrigin      ( x, y);
            setPosition    ( pos );
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
struct  SceneLogo   : Obj::IObject, TestA
{       SceneLogo() : TestA({650,400}, {0, 0}, {0,128,64})
        {
            name = "SceneLogo";
            text.setString(std::string(name));

            rs.loadTexture(Config::getFileName(2).c_str());
        }

    ///----------------------|
    /// R-спрайт.            |
    ///----------------------:
    RenderSprite           rs;

    void update   (                        ) {}
    bool RPControl(std::string_view command,
                   std::vector<float>& arg ) { return true; };

    void input    (const sf::Event&   event)
    {
        if (event.key.code == sf::Keyboard::Space)
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
        target.draw(         rs , states);

        target.setView(ConfigGame::p->getView_GUI());
        target.draw(TestA::text , states);
    }
};


///----------------------------------------------------------------------------|
/// SceneGame.
///----------------------------------------------------------------------------:
struct  SceneGame   : Obj::IObject, TestA
{       SceneGame() : TestA({300,690}, {-200, 0}, {0,0,255})
        {
            name = "SceneGame";
            text.setString(std::string(name));

            rs.loadTexture(Config::getFileName(1).c_str());
        }

    ///----------------------|
    /// R-спрайт.            |
    ///----------------------:
    RenderSprite           rs;

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
        target.draw(         rs , states);

        target.setView(ConfigGame::p->getView_GUI());
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
/// Инфа о FPS.
///----------------------------------------------------------------------------:
struct  Fps : sf::Text
{       Fps(const sf::View& view)
        {
            setFont   (ConfigGame::p->font);
            setCharacterSize           (14);
            setStyle    (sf::Text::Regular);
            setFillColor(sf::Color(255,255,0));

            setPosition({float(ConfigGame::p->winsize.x - PAD), 0});
        }

    void update()
    {
        if (f  += ConfigGame::p->deltaTime; f > 1.f)
        {   f  -= 1.f;
            uptext ();
            cnt =   1;
        }
        else    ++cnt;
    }

private:
    int  cnt =  1  ;
    float  f =  0.f;

    const float PAD = 10;

    void uptext()
    {   std::string s("FPS: ");
                    s += std::to_string(cnt);
        setString  (s);
        dub2right  ( );
    }

    void dub2right()
    {   const sf::FloatRect& BOUND = getLocalBounds();
        if(BOUND.width > ConfigGame::p->winsize.x - getPosition().x)
        {   setPosition({ConfigGame::p->winsize.x - BOUND.width - PAD, 0});
        }
    }
};


///----------------------------------------------------------------------------|
/// RenderLoop
///----------------------------------------------------------------------------:
struct  RenderLoop
{       RenderLoop() : window(sf::VideoMode(ConfigGame::p->winsize.x,
                                            ConfigGame::p->winsize.y),
            "T03::TexturesHolder"),
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
    std::wcout << "START ...\n\n";

    Programm programm;

    std::wcout << "FINISHED!\n";
}
