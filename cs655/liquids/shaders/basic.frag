#version 400

in vec3 vcolor;
out vec4 frag_colour;

void main() {
  frag_colour = vec4(vcolor, 0.1);
}