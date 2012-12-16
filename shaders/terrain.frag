
uniform float dirtMin, dirtMax;
uniform float grassMin, grassMax;
uniform float rockMin, rockMax;
uniform float snowMin, snowMax;
    
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying float height;
varying float intensity;

vec4 sampleTextures()
{
    float rockRange = rockMax - rockMin;
    float rockWeight = (rockRange - abs(height-rockMax))/rockRange;
    rockWeight = max(0.0, rockWeight);
    vec4 rockSample = texture2D(rockTexture, gl_TexCoord[0].st);
    vec4 rockComponent = rockSample*rockWeight;
    
    float dirtRange = dirtMax - dirtMin;
    float dirtWeight = (dirtRange - abs(height-dirtMax))/dirtRange;
    dirtWeight = max(0.0, dirtWeight);
    vec4 dirtSample = texture2D(dirtTexture, gl_TexCoord[0].st);
    vec4 dirtComponent = dirtSample*dirtWeight;
    
    float grassRange = grassMax - grassMin;
    float grassWeight = (grassRange - abs(height-grassMax))/grassRange;
    grassWeight = max(0.0, grassWeight);
    vec4 grassSample = texture2D(grassTexture, gl_TexCoord[0].st);
    vec4 grassComponent = grassSample*grassWeight;
 
    float snowRange = snowMax - snowMin;
    float snowWeight = (snowRange - abs(height-snowMax))/snowRange;
    snowWeight = max(0.0, snowWeight);
    vec4 snowSample = texture2D(grassTexture, gl_TexCoord[0].st);
    vec4 snowComponent = snowSample*snowWeight;
    
    //sample *= sample*rockWeight;
    return dirtComponent+rockComponent+grassComponent+snowComponent;
}

void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
