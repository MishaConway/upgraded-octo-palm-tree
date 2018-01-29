#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/textures.h"

void main(void)
{
    gl_FragColor =  SampleTex1(out_color_uv.xy);
}