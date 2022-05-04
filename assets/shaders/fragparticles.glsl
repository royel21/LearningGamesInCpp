in vec2 uv;
in vec4 color;

uniform int hasTexture;
uniform sampler2D uSampler;

out vec4 fragColor;

void main() {

    if(hasTexture > 0){
        vec4 tex = texture(uSampler, uv);
        fragColor = color * tex;
    }else{
        fragColor = color;
    }
}