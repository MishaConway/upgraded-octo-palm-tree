#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/textures.h"

void main(void)
{
    gl_FragColor =  sample_tex_1(out_color_uv.xy);
}