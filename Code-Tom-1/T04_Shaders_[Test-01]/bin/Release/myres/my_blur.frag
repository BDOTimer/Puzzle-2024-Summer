//uniform sampler2D texture  ;
uniform float     iTime      ;
uniform float     blur_radius;

const vec3  RED   = vec3(0.6, 0. , 0. );
const vec3  GREEN = vec3(0. , 0.6, 0. );
const vec3  GRAY  = vec3(0.2, 0.4, 0.6);
const vec3  WHITE = vec3(1. , 1. , 1. );

float stretch(float x, float start, float finish)
{   return (x - start)/(finish - start);
}

vec4 foo_02(vec2 pos, float radius, float speed, vec3 COLOR)
{

    float   x = radius*sin(iTime *  speed);
    float   y = radius*cos(iTime * (speed-0.2));

    float len = 2.3*length(vec2(pos.x - 0.5 + x, pos.y - 0.5 + y));

    vec4 res;
    if( (0. < len && len < .2))
    {
        float l = len * 10.;// - 1.;
        //res = vec4(GRAY, 1.) *( sin(wave_phase) + 1. - l);
        //res = vec4(GRAY, 1.)*(l + 0.3);
        res = vec4(COLOR, 1.) *( 1. - l + 0.7);

    }
    else
    {
        float l = len * 10.;// - 4.;
        //res = vec4(RED, 1.)*((sin(wave_phase*2.-l))*2.-.5);
        res = vec4(0.,0.,0., 0.);
    }

    return res;
}

void main()
{
    vec4 res = vec4(0.,0.,0., 0.);
    for(float i = 0.1; i < 0.5; i += 0.04)
    {
        res += foo_02(gl_TexCoord[0].xy, i,  i * 2.6 , GRAY );
    }

    gl_FragColor = res;
}
