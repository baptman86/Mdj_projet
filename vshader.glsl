#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform vec4 a_color;

attribute vec4 a_position;
attribute vec3 a_texinfo;

varying vec3 v_texinfo;
varying vec4 v_color;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texinfo = a_texinfo;
    v_color = a_color;

}
//! [0]
