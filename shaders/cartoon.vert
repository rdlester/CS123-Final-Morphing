varying float intensity;

void main()
{
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    //gl_Position = ftransform();
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    
    vec4 light_normal = normalize(gl_LightSource[0].position - gl_ModelViewMatrix*gl_Vertex);
    intensity = max(0.0, dot(normal, light_normal));
    intensity = min(1.0, dot(normal, light_normal));
	

}
