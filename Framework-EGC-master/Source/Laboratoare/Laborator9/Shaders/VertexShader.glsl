#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Clock;
uniform int tooning;

out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
	texcoord = v_texture_coord;
	vec3 pos = v_position;
	
	//verificam daca avem de facut efectul de tooning
	if(tooning == 1){
		//daca da modificam pozitia vertecsilor conform formulei din enunt: inmultim normala cu un factor 
		// reprezentat de produsul dintre sinus si un scalar si adunam acest rezultat pozitiei curente
		vec3 aux = v_normal * sin(Clock) * 0.80f;
		pos += aux;
	}
	world_position = (Model * vec4(pos, 1)).xyz;
	world_normal = normalize( mat3(Model) * v_normal );
	gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
