#version 330

const float GLOSSINESS_POWER = 32.0;
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float lightIntensityMult = 1.0;
const float emissiveLightMult = 0.0;

uniform vec3 u_lightWorldPos;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_specularMap;
uniform vec3 u_cameraWorldPos;
uniform mat4 u_objectToWorldMatrix;

uniform float u_time;

in vec4 v_surfaceColor;
in vec3 v_worldPosition;
in vec3 v_normal;
in vec2 v_texCoords;
in vec3 v_tangent;
in vec3 v_bitangent;

out vec4 fragColor;

vec3 DecodeXYZfromRGB(vec3 RGB) {
	return (2.0 * RGB) - 1.0;
}

vec3 EncodeRGBfromXYZ(vec3 xyz) {
	return (xyz + 1.0) * 0.5;
}

vec4 EncodeRGBAfromXYZ(vec3 xyz) {
	return vec4((xyz + 1.0) * 0.5, 1.0);
}
 
void main()
{
	vec3 tangent = normalize(v_tangent);
	vec3 bitangent = normalize(v_bitangent);
	vec3 normal = normalize(v_normal);

	vec4 diffuseTexel = texture(u_diffuseMap, v_texCoords);
	vec4 normalTexel = texture(u_normalMap, v_texCoords);
	vec4 specularTexel = texture(u_specularMap, v_texCoords);
	vec3 tangentSpaceNormal = DecodeXYZfromRGB(normalTexel.xyz);

	vec3 worldNormal = tangentSpaceNormal;
	mat3 tangentToObject;
	tangentToObject[0] = tangent;
	tangentToObject[1] = bitangent;
	tangentToObject[2] = normal;


	vec4 objectSpaceNormal = vec4(tangentToObject * tangentSpaceNormal, 1.0);
	worldNormal =  (u_objectToWorldMatrix * objectSpaceNormal).xyz;


	if (v_tangent == vec3(0.0, 0.0, 0.0))
		worldNormal = normal; //Bypass normal maps if tangent is invalid

		//Specular Lighting Calculations
	float glossiness  = specularTexel.g;
	float reflectivity = specularTexel.r;
	float emissive = specularTexel.b;
	//glossiness = 0.5;
	//reflectivity = 0.5;
	//emissive = 0.0;

	vec3 cameraToPixel = normalize(v_worldPosition - u_cameraWorldPos);
	vec3 pixelToLight = normalize(u_lightWorldPos - v_worldPosition);
	vec3 idealReflection = reflect(cameraToPixel, worldNormal);
	float glareIntensity = clamp(dot(pixelToLight, idealReflection), 0, 1);

	float specularExponent = 1.0 + (glossiness * GLOSSINESS_POWER);
	float glareAmount = reflectivity * glossiness * pow( glareIntensity, specularExponent);
	vec3 glareColor = glareAmount * lightColor;
	float directLightIntensity = lightIntensityMult * clamp(dot(pixelToLight, worldNormal), 0, 1);

	vec3 pixelColor = (diffuseTexel.rgb * v_surfaceColor.rgb * directLightIntensity);
	float pixelAlpha = diffuseTexel.a * v_surfaceColor.a;

	vec3 finalPixelColor = pixelColor + (emissive * emissiveLightMult) + glareColor;

	//fragColor = fragColor = EncodeRGBAfromXYZ(pixelToLight);
	fragColor = vec4( finalPixelColor, pixelAlpha );
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);


	//fragColor = vec4(EncodeRGBAfromXYZ(worldNormal.xyz));
	//fragColor = vec4(EncodeRGBAfromXYZ(normal.xyz));

	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);

	//fragColor = vec4(glareAmount, glareAmount, 1.0, 1.0);
	
	//fragColor = vec4(finalPixelColor, pixelAlpha);
	//fragColor = vec4(EncodeRGBfromXYZ(normal.xyz), 1.0);
	//fragColor = EncodeRGBAfromXYZ(v_worldPosition * 2.0);
	//fragColor = EncodeRGBAfromXYZ(pixelToLight);
	//directLightIntensity *= directLightIntensity;
	//directLightIntensity *= directLightIntensity;
	//float r = diffuseTexel.r * directLightIntensity;
	//float g = diffuseTexel.g * directLightIntensity;
	//float b = diffuseTexel.b * directLightIntensity;
	//	fragColor = vec4(directLightIntensity, directLightIntensity, directLightIntensity, 1.0);
//	fragColor = vec4(r, g, b, 1.0);
	//fragColor = vec4(glareIntensity, glareIntensity, 1.0, 1.0);
	//fragColor = EncodeRGBAfromXYZ(cameraToPixel);
}
