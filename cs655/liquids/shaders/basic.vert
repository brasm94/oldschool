#version 400
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;


uniform mat4 projection;
uniform mat4 modelview;
uniform vec2 screenSize;

out vec3 vcolor;

void main() {
  vec4 eyePos = modelview * vec4(pos,1.0f);
  vec4 projVoxel = projection * vec4(4.0f,4.0f,eyePos.z,eyePos.w);
  vec2 projSize = screenSize * (projVoxel.xy / projVoxel.w);
  gl_PointSize = 0.08f * projVoxel.w;// * (projSize.x + projSize.y);
  gl_Position = projection * modelview * vec4(pos,1);
  vcolor = color;
}