/**
 * basic.f.glsl
 */

varying vec2 vUV;
uniform sampler2D uTexture;
out vec4 glFragColor;

void main() {
    glFragColor = texture(uTexture, vUV);
}
