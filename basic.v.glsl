/**
 * basic.v.glsl
 */

attribute vec2 aXY;
attribute vec2 aUV;
varying vec2 vUV;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    vUV = aUV;
    mat4 pvm = uProjection * uView * uModel;
    gl_Position = pvm * vec4(aXY, 0.0, 1.0);
}
