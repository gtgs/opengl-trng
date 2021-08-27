#version 330 core
out vec4 color;

uniform sampler2D map;
uniform float ambientLightStrength;			// Ambient light strength
uniform vec3 ambientLightColor;				// Ambient color light

uniform vec3 diffuseLightPosition;			//Diffuse Light Position
uniform vec3 diffuseLightColor;				//Diffuse Light Color

uniform float specularLightStrength;		//Specular Light Strength
uniform vec3 specularLightColor;			//Specular Light Color
uniform floar specularLightFalloff;			//Specular Light Falloff

uniform vec3 viewPosition;

in vec3 _color;
in vec2 _normal;

void main() {
	//ambient lights - light bulbs, deals with overall scene brightness
	//multiply everything by this light to affect brightness, no need for normal calculations
	vec3 ambientComponent = ambientLightstrength * ambientLightColor;

	//directional / diffuse / local- this is what is immediately lighting the subject
	vec3 normalizedNormal = normalize(_normal)
	vec3 lightDir = normalize(diffuseLightPosition - fragPosition);
	float diff = max(dot(normalizedNormal, lightDir), 0.0);
	vec3 diffuseComponent = diff * diffuseLightColor;

	// specular /spotlight - this a high powered spotlight
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectedDir = reflect(-lightDir, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectedDir), 0.0), specularLightFalloff);
	vec3 specularComponent = specularLightStrength * spec * specularLightColor;

	color = vec4((ambientComponent + diffuseComponent + specularComponent) * _color, 1.0);
}
