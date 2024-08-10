#define DESKTOP_
#ifdef  DESKTOP_
    #define FC        gl_FragColor
    #define iChannel0 textureA
    #define texture   texture2D

    uniform sampler2D textureA;
    uniform float     iTime;
    uniform vec2      iResolution;
#endif

/// template
///----------------------------------------------------------------------------|
/// Той-02
///----------------------------------------------------------------------------:
const vec3  RED    = vec3(0.7, 0. , 0. );
const vec3  GREEN  = vec3(0.7, 0.7, 0.0);
const vec3  BLUE   = vec3(0.0, 0.0, 0.4);
const vec3  WHITE  = vec3(0.9, 0.9, 0.9);
const vec3  BLACK  = vec3(0.0, 0.2, 0.1);
const vec3  SHOVEL = vec3(0.2, 0.5, 0.3);

///----------------------|
/// Базовые значения.    |
///----------------------:
const float SZCELL = 25.0         ; /// Ширина сетки.
const float dim    =  2.0 * SZCELL; /// Расстояние между парой.


float r_now_ext    =  2.5 * SZCELL; /// Внешний    радиус трека.
float r_now_intro;                  /// Внутренний радиус трека.
float dd         ;                  /// Смещение трека от центра доски.


    ///------------------:
    float foot_now =  6.0;
    float foot_act =  4.0;
    ///------------------.

float foot_y;

vec2  emptiness  ; /// Прямоугольная пустотелость внутри доски.
vec2  shovel     ; /// Прямоугольный движетель-лопата.

float s_mover    ; /// Расстояние хода лопаты.

float s_mover_x = 0.0;

void calc()
{
  //r_now_ext   = ...;
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

///----------------------------------------------------------------------------:
bool iscells(in vec2 p) { return mod(p.x, SZCELL) < 1.0 || mod(p.y, SZCELL) < 1.0; }
bool isaxes (in vec2 p) { return     p.x         == 0.0 ||     p.y         == 0.0; }

bool isshovel_center(in vec2 p)
{   float  x = abs(p.x - s_mover_x);
    return x < SZCELL / 2.0 && abs(p.y) < shovel.y / 2.0;
}

bool isshovel(in vec2 p)
{   float  x = abs(p.x - s_mover_x);
    return x < shovel.x / 2.0 && abs(p.y) < shovel.y / 2.0 && x > SZCELL / 2.0;
}

bool isemptiness(in vec2 p)
{   return abs(p.x) < emptiness.x / 2.0 && abs(p.y) < emptiness.y / 2.0;
}

bool isfoots(in vec2 p, in float d)
{
    vec2   P = vec2(abs(p.x + d), abs(p.y));
    return P.y < foot_y / 2.0 && P.x < r_now_ext && P.x > r_now_intro;
}

bool istrack_half(in vec2 p, in float r1, in float r2, in float d)
{
    float x = p.x + d;
    float y = abs(p.y) - foot_y / 2.0;

    float R1 = r1 * r1;
    float R2 = r2 * r2;
    float S  = x * x + y * y;

    return S < R1 && S > R2 && y > 0.0;
}

bool istrack2(in vec2 p, in float r1, in float r2, in float d)
{
    return isfoots(p, dd) || isfoots(p, -dd) ||

           istrack_half(p, r_now_ext, r_now_intro,  dd) ||
           istrack_half(p, r_now_ext, r_now_intro, -dd);
}

///----------------------------------------------------------------------------:
void mainImage(out vec4 FC, in vec2 fragCoord)
{
    calc();

    vec2 p    = fragCoord - iResolution.xy / 2.0;
    vec2 uv   = fragCoord / iResolution.xy;

    vec4 TXTR_COLOR = texture(iChannel0, uv);

    s_mover_x = (s_mover / 2.0) * sin(iTime / 1.0);

    vec2 t    = vec2(fragCoord.y, fragCoord.x - s_mover_x) / iResolution.xy;
    vec4 F    = texture(iChannel0, t);

         if(isaxes  (   p                         )) FC = vec4(WHITE , 1.0);
    else if(iscells (   p                         )) FC = vec4(GREEN , 1.0);
    else if(isshovel_center(p                     )) FC = vec4(BLUE  , 1.0);
    else if(isshovel(   p                         )) FC = F + vec4(0.2, 0.2, 0.2, 0.0);
    else if(isemptiness(p                         )) FC = vec4(BLACK , 1.0);
    else if(istrack2(p, r_now_ext, r_now_intro, dd)) FC = vec4(BLUE  , 1.0);
    else FC = TXTR_COLOR;
}

#ifdef DESKTOP_
    void main()
    {   vec2 fragCoord = iResolution.xy * gl_TexCoord[0].xy;

        mainImage(gl_FragColor, fragCoord);
    }
#endif
