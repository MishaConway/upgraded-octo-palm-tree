uniform mat4 world_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;
uniform mat4 world_inverse_transpose;

uniform float viewport_width;
uniform float viewport_height;

uniform vec4 color;
uniform vec4 specular_color;
uniform float specular_power;

uniform vec3 eye_position;
uniform vec4 light_source;

uniform int compute_projective_texture_coordinates;

uniform vec4 clip_plane;
uniform int clipping_enabled;

