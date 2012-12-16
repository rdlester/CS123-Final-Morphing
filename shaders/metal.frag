
varying vec3 vertex;		// The vector from the eye to the vertex
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform samplerCube envMap;	// The cube map containing the environment to reflect
uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect
    
uniform float r0;		// The Fresnel reflectivity when the incident angle is 0
uniform float m;		// The roughness of the material

uniform samplerCube tex;

void main()
{
    vec3 e = normalize(eye);
    vec3 n = normalize(normal);
    vec3 i = normalize(vertex - eye);
    vec3 l = normalize(light);
    
    // TODO: fill this in
    gl_FragColor = textureCube(tex, gl_ModelViewMatrixInverse*(vec4(reflect(i,n).xyz, 0)));
    //compute the diffuse coefficient
    float intensity_d = dot(n,l);
	
    //add  specular coefficient
    vec3 r = reflect(-l, n);
    
    vec3 H = normalize(l-i);
    
    float NdotH = dot(n,H);
    float EdotN = dot(e,n);
    float EdotH = dot(e,H);
    float LdotN = dot(e,l);
    
    float alpha = acos( NdotH );
	
    float D = exp(-(tan(alpha)*tan(alpha))/(m*m))/(4*m*m*pow(cos(alpha),4));
    
    float G = min(1.0f, min(2*NdotH*EdotN/EdotH, 2*NdotH*LdotN/EdotH));
        
    float F = r0 + (1-r0)*pow(1-dot(-i,n), 5);
	
    float intensity_spec = max(0.0, D*F*G/EdotN);
    
    vec3 sum_light3 = ambient+ diffuse*intensity_d +specular*intensity_spec;
    
    vec4 sum_light4 = vec4(sum_light3.xyz, 0);
    
    gl_FragColor = max(0.0, F*gl_FragColor)+(1-F)*sum_light4;
}
