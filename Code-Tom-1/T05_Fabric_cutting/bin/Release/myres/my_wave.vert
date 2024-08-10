uniform float wave_phase;
uniform vec2  wave_amplitude;

///-------------------------|
/// My.                     |
///-------------------------:
uniform sampler2D texture   ;

void main()
{
    vec4 vertex = gl_Vertex;
    
    //vertex.y -= 20.*sin(wave_phase);
    
    gl_Position    = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    
    
    //gl_Position.y += vertex.y;
    
    ////////////////////////////////////////////////////////////
    //float factor = 1.0 / (pixel_threshold + 0.001);
    //vec2 pos = floor(gl_TexCoord[0].xy * wave_phase + 0.5) / wave_phase;
    //vec2 pos;
    //     pos.x = vertex.x;
    //     pos.y = vertex.y;
    //gl_FragColor = gl_Color * texture2D(texture, pos);
    
    
    vec2 pos;
         pos.x = vertex.x;
         pos.y = vertex.y;
    
    //gl_FrontColor  = texture2D(texture, pos);
    
    gl_FrontColor  = gl_Color;
}
