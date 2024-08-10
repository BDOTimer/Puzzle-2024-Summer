#ifndef EFFECT_T_H
#define EFFECT_T_H

#include "ieffect.h"

constexpr std::string_view PATH2RES{"../../../../Resources/"};

inline std::string getPath2Res(std::string_view filename)
{   return std::string(PATH2RES) + filename.data();
}

///----------------------------------------------------------------------------|
/// Effect_Test_01.
///----------------------------------------------------------------------------:
struct  Effect_Test_01   : public IEffect
{       Effect_Test_01() : IEffect("Effect_Test_01")
        {
        }

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

    bool onLoad()
    {   m_text.setString(L"я class Effect_Test_01;\n");
        m_text.setFont(getFont  ());
        m_text.setCharacterSize(22);
        m_text.setPosition (20, 30);

        ///----------------------------------------|
        /// My m_texture.                          |
        ///----------------------------------------:
        if (!m_texture.loadFromFile("myres/flag-01.png"))
        {    return false;
        }

        m_sprite.setTexture(m_texture );
        m_sprite.setPosition(20.f, 60.f);

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
        target.draw(m_text  , states);
    }

private:

    sf::Text    m_text   ;
    sf::Shader  m_shader ;

    sf::Texture m_texture;
    sf::Sprite  m_sprite ;
};


///----------------------------------------------------------------------------|
/// Effect_Test_03.
///----------------------------------------------------------------------------:
struct  Effect_Test_03                         : public IEffect
{       Effect_Test_03(const sf::Vector2u& sz) : IEffect("Effect_Test_03")
        {
        }

    sf::Glsl::Vec2 iResolution;

    bool onLoad()
    {   m_text.setString(L"я class Effect_Test_03;\n");
        m_text.setFont(getFont  ());
        m_text.setCharacterSize(22);
        m_text.setPosition (20, 50);

        ///----------------------------------------|
        /// My m_texture.                          |
        ///----------------------------------------:
        if (!m_texture.loadFromFile(getPath2Res("wood_01-2.jpg")))
        {    return false;
        }

        iResolution.x = m_texture.getSize().x;
        iResolution.y = m_texture.getSize().y;

        m_sprite.setTexture(m_texture);
        m_sprite.setPosition(0.f, 0.f);

        if(!m_shader.loadFromFile("myres/my_wave.vert",
                                  getPath2Res("toy-02D.frag").c_str()))
        {   return false;
        }

        m_shader.setUniform("textureA"   , sf::Shader::CurrentTexture);
        m_shader.setUniform("iResolution", iResolution               );

        return true;
    }

    void onUpdate(float time, float x, float y)
    {   static float T  = 0.f ;
                     T += time;
        m_shader.setUniform("iTime", T);
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
