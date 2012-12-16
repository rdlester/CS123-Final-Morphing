varying float intensity;

uniform float time;
    
float computeScale(float t)
{
    return 0.7+0.3*sin(t);
}
    
void main()
{
    float scale = computeScale(time);
    
    vec4 vertex = gl_Vertex;
    vertex.z = scale*vertex.z;
    gl_Position = gl_ModelViewProjectionMatrix*vertex;
    
    // Flatten the normals to make the lighting a little more realistic
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal * vec3(1.0, 1.0, 1.0 / scale));
    vec3 light = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * vertex)).xyz;
    intensity = max(0.0, dot(normal, light));
} 
