#define SFML_STATIC
///----------------------------------------------------------------------------|
/// Tutor-08::Траектория[Идея]
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <list>

#include <SFML/Graphics.hpp>

#define l(v) std::wcout << #v << " = " << (v) << "\n";

const float PI   = 3.14159f;
const float PIx2 = 3.14159f * 2;
const float PId2 = 3.14159f / 2;


///----------------------------------------------------------------------------|
/// Конфиг траектории.
///----------------------------------------------------------------------------:
struct  ConfigTraj
{       ConfigTraj()
        {     calc();
        }

    ///----------------------|
    /// Базовые значения.    |
    ///----------------------:
    const float SZCELL = 25.0         ; /// Ширина сетки.
    const float dim    =  2.0 * SZCELL; /// Расстояние между парой.

    float r_now_ext    =  2.5 * SZCELL; /// Внешний    радиус трека.
    float r_now_intro;                  /// Внутренний радиус трека.
    float dd         ;                  /// Смещение трека от центра доски.

    ///----------------------:
    float foot_now     =  6.0;
    float foot_act     =  4.0;
    ///----------------------.

    float foot_y;

    sf::Vector2f  emptiness  ; /// Прямоугольная пустотелость внутри доски.
    sf::Vector2f  shovel     ; /// Прямоугольный движетель-лопата.

    float s_mover            ; /// Расстояние хода лопаты.
    float s_mover_x     = 0.0;

    ///----------------------|
    /// Инстанс конфига.     |
    ///----------------------:
    inline static ConfigTraj* p = nullptr;

    ///----------------------|
    /// Инициализация.       |
    ///----------------------:
    static void init()
    {
        static ConfigTraj configTraj; p = &configTraj;
    }

    void calc()
    {
    //  r_now_ext   = ...;
        r_now_intro = r_now_ext - SZCELL;

        dd          = r_now_ext + dim / 2.0;
        ///--------------------------------.

        foot_y      = SZCELL * foot_now;

        emptiness.x = SZCELL * 5.0 + dim * 2.0;
        emptiness.y = SZCELL * foot_act;

        shovel.x    = SZCELL * 2.0 + dim * 2.0;
        shovel.y    = emptiness.y;

        s_mover     = SZCELL + dim;
    }

    float FPS = 60.f;
};


struct PosBall : sf::Vector2f
{
    bool isshovel = false;
};


///----------------------------------------------------------------------------|
/// Траектория. (в данном тестовом случае - траектория это окружность)
///----------------------------------------------------------------------------:
struct  Trajectory : std::vector<PosBall>
{       Trajectory(const sf::Vector2f p) : center(p)
        {
            calc();
        }

    /// TODO...

private:
    sf::Vector2f center;

    void calc()
    {
        const auto CFG = ConfigTraj::p;

        const float R   = CFG->r_now_intro + CFG->SZCELL / 2.f;
        const float L   = PI * (R + R);
        const float A   = PIx2 / L;

        for(float a = 0.0; a < PIx2; a += A)
        {
            sf::Vector2f p{R * cosf(a) + center.x,
                           R * sinf(a) + center.y};
            push_back  ({p});
        }
    }

public:
    std::list<PosBall> getPosSet(const unsigned N)
    {
        const std::vector<PosBall>& T = (*this);

        std::list<PosBall>   u;
        size_t iD = size() / N;

        for(size_t i = 0, ll = 0; i < N; ++i, ll += iD )
        {   u.push_back({T[norm(ll)]});
        }

        return u;
    }

    size_t norm(size_t i){ return i % size(); }
};


///----------------------------------------------------------------------------|
/// Тестовый примитив(для визуализатора).
///----------------------------------------------------------------------------:
struct  Ball : sf::CircleShape
{       Ball()
        {   setRadius(ConfigTraj::p->SZCELL / 2.f);
            setFillColor(sf::Color(224,0,0));
            setOutlineColor(sf::Color(0,100,0) );
            setOutlineThickness              (2.f);

            setOrigin({ConfigTraj::p->SZCELL / 2.f,
                       ConfigTraj::p->SZCELL / 2.f});
        }
};

struct  Track : sf::CircleShape
{       Track()
        {   setOutlineColor(sf::Color(0,100,0) );

            float R = ConfigTraj::p->r_now_ext - ConfigTraj::p->SZCELL;

            setRadius(R);
            setOrigin(R, R);
            setOutlineThickness(ConfigTraj::p->SZCELL);
            setFillColor(sf::Color(0,128,0));
        }
};


///----------------------------------------------------------------------------|
/// Визуализатор.
///----------------------------------------------------------------------------:
struct  Run
{       Run() : window(sf::VideoMode(800, 600), L"Tutor-08::Траектория[Идея]"),
                a({-200, 0}),
                b({ 200, 0})

        {
            ///----------------------|
            /// Камеры вида.         |
            ///----------------------:
            cam_ui    = window.getView();
            cam_world = cam_ui;
            cam_world.setCenter({0,0});

            hero.setOrigin      ({350, 225});
            hero.setFillColor   (sf::Color( 0,64,0));
            hero.setSize        ({700, 450}        );
            hero.setOutlineColor(sf::Color(128,0,0));
            hero.setOutlineThickness           (2.f);

            font.loadFromFile   ("c:/windows/fonts/georgia.ttf"); ///<-------!!!

            text.setFont                 (font);
            text.setCharacterSize          (18);
            text.setStyle   (sf::Text::Regular);
            text.setFillColor(sf::Color::White);

            track_a.setPosition({-200, 0});
            track_b.setPosition({ 200, 0});

            loop();
        }

    sf::RenderWindow window;
    sf::RectangleShape hero;
    sf::Font           font;
    sf::Text           text;

    Ball               ball;
    Trajectory          a,b;

    Track           track_a,
                    track_b;

    ///--------------------|
    /// Камеры вида.       |
    ///--------------------:
    sf::View      cam_world;
    sf::View      cam_ui   ;

    void process_mouse(const sf::Vector2i& mouse_pos)
    {   std::string s("mouse::XY = [");
                    s += std::to_string(mouse_pos.x) + ", ";
                    s += std::to_string(mouse_pos.y) + "]" ;
        text.setString(s);
    }

    void loop()
    {
        sf::Vector2i mouse_pos;

        auto ta = a.getPosSet(5);
        auto tb = b.getPosSet(8);

        l(ta.size())
        l(tb.size())

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

            ///----------------------|
            /// cam_world.           |
            ///----------------------:
            window.setView(cam_world);

            window.draw(hero);

            window.draw(track_a);
            window.draw(track_b);

            for(const auto& e : ta){ ball.setPosition({e}); window.draw(ball); }
            for(const auto& e : tb){ ball.setPosition({e}); window.draw(ball); }

            ///----------------------|
            /// cam_ui.              |
            ///----------------------:
            window.setView   (cam_ui);
            window.draw        (text);

            window.display         ();
        }
    }
};

///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int main()
{
    std::cout << "START ...\n";

    ConfigTraj::init();

    std::unique_ptr<Run> run(new Run);

    std::cout << "PROGRAMM fINISHED\n";
}
