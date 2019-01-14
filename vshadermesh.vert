#version 400 core
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;

uniform vec3 light_color;
uniform vec3 light_position;

in vec3 a_position;
in vec2 a_texcoord;
in vec3 a_normal;
in vec3 a_color;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 the_color;
out vec3 frag_pos;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = p_matrix * v_matrix * m_matrix * vec4(a_position,1.0);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    the_color = a_color;
    v_texcoord = a_texcoord;
    v_normal = a_normal;
    frag_pos = vec3(m_matrix * vec4(a_position,1.0));
}
