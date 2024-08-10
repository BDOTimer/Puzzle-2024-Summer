/// template
///-------------------------------------------------------------------------------------|
/// Той-01
///-------------------------------------------------------------------------------------:
const vec3  RED    = vec3(0.7, 0. , 0. );
const vec3  GREEN  = vec3(0.7, 0.7, 0.0);
const vec3  BLUE   = vec3(0.0, 0.0, 0.4);
const vec3  WHITE  = vec3(0.9, 0.9, 0.9);

///----------------------|
/// Базовые значения.    |
///----------------------:
const float SZCELL = 25.0         ; /// Ширина сетки.
const float dim    =  2.0 * SZCELL; /// Расстояние между парой.


float r_now_ext    = 2.5 * SZCELL;  /// Внешний    радиус трека.
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

void calc()
{
  //r_now_ext   = ...;
    r_now_intro = r_now_ext - SZCELL;
    
    
    dd          = r_now_ext + dim / 2.0;
    ///--------------------------------.

    foot_y      = SZCELL * foot_now;
        
    emptiness.x = SZCELL * 4.0 + dim * 2.0;
    emptiness.y = SZCELL * foot_act;
        
    shovel.x    = SZCELL * 2.0 + dim * 2.0;
    shovel.y    = emptiness.y;
        
    s_mover     = SZCELL + dim;
    
    
}

///-------------------------------------------------------------------------------------:
bool iscells(in vec2 p) { return mod(p.x, SZCELL) < 1.0 || mod(p.y, SZCELL) < 1.0; }
bool isaxes (in vec2 p) { return     p.x         == 0.0 ||     p.y         == 0.0; }

bool istrack(in vec2 p, in float r1, in float r2, in float d)
{   
    float x = p.x + d;

    float R1 = r1 * r1;
    float R2 = r2 * r2;
    float S  = x * x + p.y * p.y;
    
    return S < R1 && S > R2;
}
bool istrack2(in vec2 p, in float r1, in float r2, in float d)
{  
    return istrack(p, r_now_ext, r_now_intro,  dd) ||
           istrack(p, r_now_ext, r_now_intro, -dd);
}

///-------------------------------------------------------------------------------------:
void mainImage(out vec4 FC, in vec2 fragCoord)
{
    calc();

    vec2 p  = fragCoord - iResolution.xy / 2.0;
    vec2 uv = fragCoord/iResolution.xy;

         if(isaxes  (p                            )) FC = vec4(WHITE, 1.0);
    else if(iscells (p                            )) FC = vec4(GREEN, 1.0);
    else if(istrack2(p, r_now_ext, r_now_intro, dd)) FC = vec4(BLUE , 1.0);
    else FC = vec4(texture(iChannel0, uv));
}