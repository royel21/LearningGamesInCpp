in vec2 uv;

uniform vec4 uColor;
uniform int hasTexture;
uniform sampler2D uSampler;

out vec4 fragColor;

void main() {

    if(hasTexture > 0){
        vec4 tex = texture(uSampler, uv);
        fragColor = uColor * tex;
    }else{
        fragColor = uColor;
    }
}