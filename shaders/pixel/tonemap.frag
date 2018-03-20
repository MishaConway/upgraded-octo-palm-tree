#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"
#include "includes/hdr/tone_mapping.h"


void main()
{
    vec4 color = sample_tex_1( out_color_uv.xy ) * 3.0;
    vec4 tonemapped_color = vec4( Uncharted2ToneMapping( color.rgb ), color.a );
    vec4 gamma_color = gamma_correct(tonemapped_color);
    gl_FragColor = tonemapped_color;
}