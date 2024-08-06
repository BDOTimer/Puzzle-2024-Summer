#ifndef FPS_H
#define FPS_H
///----------------------------------------------------------------------------|
/// "fps.h"
///----------------------------------------------------------------------------:

#include "log.h"

///----------------------------------------------------------------------------|
/// Инфа о FPS.
///----------------------------------------------------------------------------:
struct  Fps : sf::Text
{       Fps(const sf::View& view)
        {
            setFont   (ConfigRender::p->font);
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

#endif // FPS_H
