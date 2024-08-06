#ifndef GAME_H
#define GAME_H
///----------------------------------------------------------------------------|
/// "game.h"
///----------------------------------------------------------------------------:

#include "core.h"

///----------------------------------------------------------------------------|
/// SceneGame.
///----------------------------------------------------------------------------:
struct  SceneGame   : Obj::IObject, TestA
{       SceneGame() :
            TestA({300,690}, {-200, 0}, {0,0,255}),
            rs   (ConfigGame::p->winsize)
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
        target.draw(         rs , states);
        target.draw(TestA(*this), states);

        target.setView(ConfigGame::p->getView_GUI());
        target.draw(TestA::text , states);
    }
};

#endif // GAME_H
