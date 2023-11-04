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

out vec2 UVs;
out vec3 outNormals;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    UVs = texCoords;
    outNormals = (modelMatrix * vec4(normals, 0.0)).xyz;
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

out vec2 UVs;
out vec3 outNormals;

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
}
'''
fragmentation_shader1 = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;

out vec2 UVs;
out vec3 outNormals;

void main()
{
    UVs = texCoords;
    outNormals = (modelMatrix * vec4(normals,0.0)).xyz;  
    outNormals = normalize(outNormals);
    float x = position.x +  150.0f * sin(noise(100));
    float y = position.y +  150.0f * cos(noise(100));
     
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(x, y, position.z, 1.0);
}
'''
fragmentation_shader = '''
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;

out vec2 UVs;
out vec3 outNormals;

void main()
{
    UVs = texCoords;
    outNormals = (modelMatrix * vec4(normals,0.0)).xyz;  
    outNormals = normalize(outNormals);
    float x = position.x - normals.x * 0.1;
    float y = position.y - normals.y * 0.1;
    float z = position.z - normals.z * 0.1;
     
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(x, y, z, 1.0);
}
'''

inverse_shader = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

in vec2 UVs;
in vec3 outNormals;

out vec4 fragColor;

void main()
{
    float intensity = dot(outNormals, -dirLight);
    vec4 originalColor = texture(tex, UVs);
    vec4 invertedColor = 1.0 - originalColor;
    fragColor = invertedColor;
}
'''
fragment_shader = '''
#version 450 core

layout (binding = 0) uniform sampler2D tex;

uniform vec3 dirLight;

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

in vec2 UVs;
in vec3 outNormals;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, UVs);
}
'''