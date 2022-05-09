
out vec4 fragColor;


uniform vec4 uColor;
uniform int hasTexture;

uniform sampler2D mySampler;
uniform sampler2D mySampler2;

in vec2 uv;
in float textureIndex;

void main(){
    if(hasTexture == 1){
        if(textureIndex == 1){
            vec4 tex = texture(mySampler, uv);
            fragColor = tex * uColor;
         }else{
            vec4 tex = texture(mySampler2, uv);
            fragColor = tex * uColor;
         }
    }else{
        fragColor = uColor;
    }
}