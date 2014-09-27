#version 150

//////////////////////////////////////////////////////////////////////
// iloczyn macierzy modelu-widoku i macierzy rzutowania
//////////////////////////////////////////////////////////////////////
uniform mat4 V;
uniform mat4 P;
uniform vec3 worldpos;
uniform vec3 LightPosition_worldspace;
uniform vec3 myNormal;

//////////////////////////////////////////////////////////////////////
// wspó³rzêdne wierzcho³ka
//////////////////////////////////////////////////////////////////////
in vec3 in_Position;
in vec2 vertexUV;

out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 UV;

void main()
{
    // standard vertex transformation
	Position_worldspace = worldpos + in_Position;
	gl_Position = (P * V) * vec4(Position_worldspace, 1.0f);

	vec3 vertexPosition_cameraspace = ( V * vec4(Position_worldspace, 1.f)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	Normal_cameraspace = ( V * vec4(Position_worldspace + myNormal, 0)).xyz;

	UV = vec3(mod(vertexUV.x,2), int(vertexUV.x)/2, vertexUV.y);
}
