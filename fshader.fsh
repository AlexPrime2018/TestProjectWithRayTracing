uniform sampler2D u_texture;
uniform vec4 u_lightPosition;
uniform float u_lightPower;
varying vec4 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));
    float len  = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = 60.0;
    float ambientFactor = 0.1;

    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect));
    resultColor += diffColor;
    vec4 ambientColor = diffMatColor * ambientFactor;
    resultColor += ambientColor;

    //vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);
    //resultColor += specularColor;

    gl_FragColor = resultColor;
}
