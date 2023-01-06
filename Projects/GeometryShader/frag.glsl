
in vec2 uv;
uniform vec4 uColor;
uniform sampler2D mySampler;

out vec4 fragColor;

void main(){
    vec4 tex = texture(mySampler, uv);
    fragColor = tex * uColor;
}