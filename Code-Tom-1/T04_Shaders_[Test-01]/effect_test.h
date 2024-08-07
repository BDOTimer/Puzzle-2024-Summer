﻿#ifndef EFFECT_T_H
#define EFFECT_T_H

#include "ieffect.h"

///----------------------------------------------------------------------------|
/// Effect_Test_01.
///----------------------------------------------------------------------------:
class   Effect_Test_01 : public IEffect
{
    std::string shader_flag
    {
        "uniform sampler2D texture;                                    "
        "uniform float       iTime;                                    "
        "                                                              "
        "void main()                                                   "
        "{                                                             "
        "    vec3  n;                                                  "
        "    float intensity = dot(vec3(gl_LightSource[0].position),n);"
        "                                                              "
        "    float x = gl_TexCoord[0].x;                               "
        "    float y = gl_TexCoord[0].y;                               "
        "    float a = (sin(6.28*x+3.*iTime)) * 0.1;                   "
        "    float b = (sin(3.28*y+3.*iTime)) * 0.1;                   "
        "                                                              "
        "    vec2 pos = vec2(                                          "
        "         x - 0.4*b* sin(1.57*x),                              "
        "         y + a * sin(1.57*x) - 0.1*sin(1.57*x));              "
        "                                                              "
        "    vec4 color;                                               "
        "         color.r = a*2.2;                                     "
        "         color.g = color.r;                                   "
        "         color.b = color.r;                                   "
        "         color.a = 0.0;                                       "
        "                                                              "
        "    vec4     img = texture2D(texture, pos);                   "
        "    gl_FragColor = img + color;                               "
        "}                                                             "
    };

public:

    Effect_Test_01() : IEffect("Effect_Test_01")
    {
    }

    bool onLoad()
    {   m_text.setString(L"я class Effect_Test_01;\n");
        m_text.setFont(getFont  ());
        m_text.setCharacterSize(22);
        m_text.setPosition (30, 20);

        ///----------------------------------------|
        /// My m_texture.                          |
        ///----------------------------------------:
        if (!m_texture.loadFromFile("myres/flag-01.png"))
        {    return false;
        }

        m_sprite.setTexture(m_texture );
        m_sprite.setPosition(50.f, 30.f);

        if (m_shader.loadFromMemory(       shader_flag,
                                           sf::Shader::Type::Fragment ))
        {   m_shader.setUniform("texture", sf::Shader::CurrentTexture );
        }
            else return false;

        return true;
    }

    void onUpdate(float time, float x, float y)
    {   static float T  = 0.f ;
                     T += time;
        m_shader.setUniform("iTime", T);
    //  m_shader.setUniform("blur_radius", (x + y) * 0.008f);
    }

    void onDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {   states.shader = &m_shader;
        target.draw(m_sprite, states);

        states.shader = 0;
        target.draw(m_text       , states);
    }

private:

    sf::Text    m_text   ;
    sf::Shader  m_shader ;

    sf::Texture m_texture;
    sf::Sprite  m_sprite ;
};


///----------------------------------------------------------------------------|
/// Effect_Test_02.
///----------------------------------------------------------------------------:
struct  Effect_Test_02 : public IEffect
{       Effect_Test_02(const sf::Vector2u& sz) : IEffect("Effect_Test_02")
        {   m_texture.create(sz.x, sz.y);
        }

    bool onLoad()
    {   m_text.setString(L"я class Effect_Test_02;\n");
        m_text.setFont(getFont  ());
        m_text.setCharacterSize(22);
        m_text.setPosition (30, 40);

        m_sprite.setTexture(m_texture);
        m_sprite.setPosition(0.f, 0.f);

        if(!m_shader.loadFromFile("myres/my_wave.vert",
                                  "myres/my_blur.frag"))
        {   return false;
        }

        m_shader.setUniform("texture", sf::Shader::CurrentTexture);

        return true;
    }

    void onUpdate(float time, float x, float y)
    {   static float T  = 0.f ;
                     T += time;
        m_shader.setUniform("iTime", T);
    //  m_shader.setUniform("blur_radius", (x + y) * 0.008f);
    }

    void onDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {   states.shader = &m_shader;
        target.draw(m_sprite, states);

        states.shader = 0;
        target.draw(m_text       , states);
    }

private:

    sf::Text    m_text   ;
    sf::Shader  m_shader ;

    sf::Texture m_texture;
    sf::Sprite  m_sprite ;
};

#endif // EFFECT_T_H
