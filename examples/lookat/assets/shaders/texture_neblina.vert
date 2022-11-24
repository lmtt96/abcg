#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightDirWorldSpace;

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;
out vec3 fragPObj;
out vec3 fragNObj;

out float visibility;

const float density = 0.06;
const float gradient = 2.0;

void main() {
  vec4 positionRelativeToCam = viewMatrix * modelMatrix * vec4(inPosition, 1);
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;
  vec3 L = -(viewMatrix * lightDirWorldSpace).xyz;

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;
  fragPObj = inPosition;
  fragNObj = inNormal;

  float distance = length(positionRelativeToCam.xyz);

  visibility = exp(-pow((distance * density), gradient));
  if(visibility < 0.0) {
    visibility = 0.0;
  } else if(visibility > 1.0) {
    visibility = 1.0;
  }
  gl_Position = projMatrix * vec4(P, 1.0);
}
