#ifndef RES_H
#define RES_H
///----------------------------------------------------------------------------|
/// "core.h"
///----------------------------------------------------------------------------:

#include "log.h"
#include "res.h"

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
/// Конфиг рендера.
///----------------------------------------------------------------------------:
struct  ConfigRender
{
    sf::Font font;

    static inline ConfigRender* p = nullptr;
    static void init()
    {   p->font.loadFromFile( "c:/windows/fonts/tahoma.ttf");
    }
};


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
{       RenderSprite(sf::Vector2i Szr) : szr(Szr) /// ConfigGame::p->winsize
        {

        }

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

    const sf::Vector2i szr; /// Размеры R-текстуры.
          sf::Vector2u szt; /// Размеры жпег-картинки.
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

            if(ConfigRender::p == nullptr) throw("ConfigRender::p == nullptr");

            text.setFont     (ConfigRender::p->font);
            text.setCharacterSize               (50);
            text.setStyle        (sf::Text::Regular);
            text.setFillColor     (sf::Color::Green);
            text.setPosition                ({0,30});
        }

        sf::Text text;
};


#endif // RES_H
