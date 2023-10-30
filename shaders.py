# En OpenGL, los shaders se escriben en un 
# nuevo lenguaje de programacion llamado GLSL
# Graphics Library Shader Language

vertex_shader = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

out vec4 outColor;

void main()
{
    gl_Position = vec4(position, 1.0);
    outColor = vec4(inColor, 1.0);
}

'''

fragment_shader = '''
#version 450 core

in vec4 outColor;
out vec4 fragColor;

void main()
{
    fragColor = outColor;
}
'''