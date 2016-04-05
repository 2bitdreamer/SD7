#version 330

uniform float u_time;
uniform sampler2D u_diffuseMap;

in vec4 v_surfaceColor;
in vec2 v_texCoords;
out vec4 fragColor;


void main()
{
	vec4 diffuseTexel = texture(u_diffuseMap, v_texCoords);
	vec3 pixelColor = (diffuseTexel.rgb * v_surfaceColor.rgb);
	float pixelAlpha = v_surfaceColor.a * diffuseTexel.a;
	fragColor = vec4(pixelColor, pixelAlpha);
	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
