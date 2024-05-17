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
    gl_Position = vec4(aXY, 0.0, 1.0);
}
