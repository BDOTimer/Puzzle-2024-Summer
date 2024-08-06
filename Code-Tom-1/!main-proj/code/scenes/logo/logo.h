#ifndef LOGO_H
#define LOGO_H
///----------------------------------------------------------------------------|
/// "logo.h"
///----------------------------------------------------------------------------:

#include "core.h"

///----------------------------------------------------------------------------|
/// SceneLogo.
///----------------------------------------------------------------------------:
struct  SceneLogo   : Obj::IObject, TestA
{       SceneLogo() :
            TestA({650,400}, {0, 0}, {0,128,64}),
            rs   (ConfigGame::p->winsize)
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
        target.draw(         rs , states);
        target.draw(TestA(*this), states);

        target.setView(ConfigGame::p->getView_GUI());
        target.draw(TestA::text , states);
    }
};

#endif // LOGO_H
