#version 400 core

uniform sampler2D texture;

uniform vec3 light_color;
uniform vec3 light_position;
uniform bool enable_light;
uniform bool enable_texture;
uniform vec4 color;


in vec2 v_texcoord;
in vec3 v_normal;
in vec3 the_color;
in vec3 frag_pos;

void main()
{

    vec4 c;
    if (enable_texture)
    {
        c = texture2D(texture, v_texcoord)*color+texture2D(texture, v_texcoord);
    }
    else
    {
        c = vec4(the_color, 1.0);
    }

    if (enable_light)
    {
        vec3 norm = normalize(v_normal);
        vec3 lightDir = normalize(light_position - frag_pos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light_color;
        float ambientStrength = 0.4;
        vec3 ambient = ambientStrength * light_color;
        vec4 result = c * (vec4(ambient, 1.0) + vec4(diffuse, 1.0));
        gl_FragColor = result;
    }
    else
    {
        gl_FragColor = c;
    }

    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}
