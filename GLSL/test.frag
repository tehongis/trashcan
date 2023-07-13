#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;

float circleshape(vec2 position, float radius) {
    return smoothstep(radius,length(position-vec2(0.5)),0.22);
}

void main() {
    vec2 position = gl_FragCoord.xy / u_resolution;
    vec3 color = vec3(1.0,1.0,1.0);

    float circle = circleshape(position,0.37);
    color = vec3(circle)*0.5;

    float circle2 = circleshape(position-vec2(-0.1,+0.1),0.1);
    color += vec3(circle2)*0.5;

    gl_FragColor = vec4(color, 1.0);

}

