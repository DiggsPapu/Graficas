# En OpenGL, los shaders se escriben en un 
# nuevo lenguaje de programacion llamado GLSL
# Graphics Library Shader Language

vertex_shader = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;
uniform float intensidad;

out vec2 UVs;
out vec3 outNormals;
out float time2;
out float intensidad2;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    UVs = texCoords;
    outNormals = (modelMatrix * vec4(normals, 0.0)).xyz;
    time2 = time;
    intensidad2 = intensidad;
}
'''

twistDeformationShader = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float time;
uniform float intensidad;

out vec2 UVs;
out vec3 outNormals;
out float time2;
out float intensidad2;

void main()
{
    float centerX = 0.5f;
    float centerY = 0.5f;
    float dx = position.x - centerX;
    float dy = position.y - centerY;
    float radius = sqrt(dx * dx + dy * dy);
    float angle = sin(time) * radius;
    float newX = dx * cos(angle) - dy * sin(angle) + centerX;
    float newY = dx * sin(angle) + dy * cos(angle) + centerY;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newX, newY, position.z, 1.0);
    UVs = texCoords;
    outNormals = (modelMatrix * vec4(normals, 0.0)).xyz;
    time2 = time;
    intensidad2 = intensidad;
}
'''
fragmentation_shader1 = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in vec2 UVs;
in vec3 outNormals;
in float time2;
in float intensidad;

out vec4 fragColor;

void main()
{    
    fragColor = texture(tex, UVs);
    if (
        (
               abs(fragColor.r*sin(time2))<0.2
            && abs(fragColor.b*cos(time2))<0.2
        )
        ||
        (
               abs(fragColor.r*sin(time2))>0.5
            && abs(fragColor.r*cos(time2))<0.55
            && abs(fragColor.b*cos(time2))>0.5
            && abs(fragColor.b*cos(time2))<0.55
        )
    )
    {
        discard;
    }
    fragColor = texture(tex, UVs);
}
'''
big_shader = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;
uniform float intensidad;

out vec2 UVs;
out vec3 outNormals;
out float intensidad2;

void main()
{
    UVs = texCoords;
    intensidad2 = intensidad;
    outNormals = (modelMatrix * vec4(normals,0.0)).xyz;  
    outNormals = normalize(outNormals);
    float x = position.x - normals.x * sin(time);
    float y = position.y - normals.y * sin(time);
    float z = position.z - normals.z * sin(time);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(x, y, z, 1.0);
}
'''

inverse_shader = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in vec2 UVs;
in vec3 outNormals;
in float intensidad;
in float time2;

out vec4 fragColor;

void main()
{
    float intensity = dot(outNormals, -dirLight);
    vec4 originalColor = texture(tex, UVs);
    
    vec4 invertedColor; // Declare invertedColor outside the conditional blocks

    if (sin(time2)>0 )
    {
        invertedColor = 1.0 - originalColor - sin(time2)/2;
    }
    else if (sin(time2)<0)
    {
        invertedColor = 1.0 - originalColor - abs(sin(time2))/2;
    }
    fragColor = invertedColor;
}
'''
fragment_shader = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in float intensidad;
in vec2 UVs;
in vec3 outNormals;

out vec4 fragColor;

void main()
{
    
    float intensity = dot(outNormals, -dirLight);
    
    fragColor = texture(tex, UVs) * max(0, (min(1, intensity)));
}
'''
fragment_shader0 = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in float intensidad;
in vec2 UVs;
in vec3 outNormals;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, UVs);
}
'''
temperature_shader = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in float intensidad;
in vec2 UVs;
in vec3 outNormals;

out vec4 fragColor;

void main()
{
    float intensity = dot(outNormals, -dirLight);
    vec4 originalColor = texture(tex, UVs);
    
    // Remap intensity to [0, 1]
    intensity = clamp(intensity, 0.0, 1.0);

    // Calculate the new color based on the intensity
    vec3 newColor; 
    if (intensity <= 0.333){
        newColor = vec3(0.1 * originalColor.x * intensity, intensity / 0.33, 1.0);
    } 
    else if (intensity <= 0.66) {
        newColor = vec3(0.1 * originalColor.x * intensity, 1.0, (intensity - 0.33) / 0.33);
    } else {
        newColor = vec3(1.0, (intensity - 0.66) / 0.33, 0.1 * originalColor.z * intensity);
    }
    fragColor = vec4(newColor, originalColor.a);
}

'''
wave_draw_lines = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in vec2 UVs;
in vec3 outNormals;
in float time2;
in float intensidad;

out vec4 fragColor;

void main()
{    
    float intensity = dot(outNormals, -dirLight);
    vec4 originalColor = texture(tex, UVs);
    vec2 coord = vec2(UVs.x/940 * 540, UVs.y/940 * 540);
    float color = 0.0;
    color += sin(coord.x * 6.0 + sin(time2+coord.y*90.0 + cos(coord.x * 30.0 + time2 * 2.0) ) ) * 0.5;
    vec3 newColor = vec3(color * originalColor.x, color * originalColor.y, color * originalColor.z);
    fragColor = vec4(newColor,1.0);
}
'''
skybox_vertex_shader = '''
#version 450 core

layout (location = 0) in vec3 inPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 texCoords;

void main()
{
    texCoords = inPosition;
    gl_Position = projectionMatrix * viewMatrix * vec4(inPosition,1.0);
}
'''
skybox_fragment_shader = '''
#version 450 core

uniform samplerCube skybox;

in vec3 texCoords;

out vec4 fragColor;

void main() 
{
    fragColor = texture(skybox, texCoords);
}
'''