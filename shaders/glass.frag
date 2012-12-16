
varying vec3 vertex;		// The position of the vertex, in eye space
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform float r0;		// The R0 value to use in Schlick's approximation
uniform float eta1D;		// The eta value to use initially
uniform vec3  eta;		// Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform samplerCube envMap;

void main()
{
    vec3 n = normalize(normal);
    vec3 e = normalize(vertex);
    vec3 i = normalize(vertex - eye);
    vec3 l = normalize(light);
    
    //t is direction
    vec3 t_r = gl_ModelViewMatrixInverse*vec4(refract(e, n, eta.r), 0.0);
    vec3 t_g = gl_ModelViewMatrixInverse*vec4(refract(e, n, eta.g), 0.0);
    vec3 t_b = gl_ModelViewMatrixInverse*vec4(refract(e, n, eta.b), 0.0);
    
    // TODO: fill this in
    
    vec4 refrection = vec4(textureCube(envMap, t_r).r,textureCube(envMap, t_g).g,textureCube(envMap, t_b).b, 1.0);
    
    float F = r0 + (1-r0)*pow(1-dot(-i,n), 5);
   
    gl_FragColor = F*textureCube(envMap, gl_ModelViewMatrixInverse*(vec4(reflect(i,n).xyz, 0))) + (1-F)*refrection;

    
    
}
