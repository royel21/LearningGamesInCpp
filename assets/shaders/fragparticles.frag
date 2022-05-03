in vec2 uv;

uniform vec4 uColor;
uniform int hasTexture;
uniform sampler2D mySampler;

out vec4 fragColor;

void main() {

    if(hasTexture){
        vec4 tex = texture(mySampler, uv);
        fragColor = color * tex;
    }else{
        fragColor = color;
    }
}