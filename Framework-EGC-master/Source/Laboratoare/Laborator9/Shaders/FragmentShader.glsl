#version 330

//cele doua texturi
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

//pozitiile si culorile luminilor
uniform vec3 light_color0;
uniform vec3 light_position0;

uniform vec3 light_color1;
uniform vec3 light_position1;

uniform vec3 light_color2;
uniform vec3 light_position2;

uniform vec3 light_color3;
uniform vec3 light_position3;

//pozitia camerei
uniform vec3 eye_position;

//propriettile de material
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

/**
ce obiect avem de desenat:
0 - 3: lumini
4 = santa
5 = planul
6 = cerul
7 = backface santa
**/
uniform int object_to_draw;
//culoarea obiectelor
uniform vec3 obj_color;
//numarul de nivele pentru efectul de stepped lighting
uniform float numLevels;

layout(location = 0) out vec4 out_color;

void main()
{
	//calculam vectorii L, H si V pentru luminile primite
	vec3 L0 = normalize( light_position0 - world_position);
	vec3 L1 = normalize( light_position1 - world_position);
	vec3 L2 = normalize( light_position2 - world_position);
	vec3 L3 = normalize( light_position3 - world_position);

	vec3 V = normalize( eye_position - world_position);

	vec3 H0 = normalize( L0 + V );
	vec3 H1 = normalize( L1 + V );
	vec3 H2 = normalize( L2 + V );
	vec3 H3 = normalize( L3 + V );

	//calculam intensitatile difuze pentru cele 4 lumini
	float diffuse_light0_aux = material_kd * max(dot(world_normal, L0), 0);
	float diffuse_light0 = diffuse_light0_aux;

	//aplicam efectul de stepped lighting
	if(numLevels != 0){
		diffuse_light0 = floor(diffuse_light0_aux * numLevels) / numLevels;	
	}
	
	float diffuse_light1_aux = material_kd * max(dot(world_normal, L1), 0);
	float diffuse_light1 = diffuse_light1_aux;
	if(numLevels != 0){
		diffuse_light1 = floor(diffuse_light1_aux * numLevels) / numLevels;	
	}
	
	float diffuse_light2_aux = material_kd * max(dot(world_normal, L2), 0);
	float diffuse_light2 = diffuse_light2_aux;
	if(numLevels != 0){
		diffuse_light2 = floor(diffuse_light2_aux * numLevels) / numLevels;	
	}
	
	float diffuse_light3_aux = material_kd * max(dot(world_normal, L3), 0);
	float diffuse_light3 = diffuse_light3_aux;
	
	if(numLevels != 0){
		diffuse_light3 = floor(diffuse_light3_aux * numLevels) / numLevels;	
	}

	//calculam intensitatea ambientala
	float ambient_light = 1.25 * material_kd;

	//calculam intensitatea speculara si aplicam efectul de stepped lighting
	float specular_light0_aux = 0;
	float specular_light0 = 0;

	if (diffuse_light0 > 0)
	{
		specular_light0_aux =  material_ks * pow(max(dot(world_normal, H0), 0), material_shininess);
		if(numLevels != 0){
			specular_light0 = floor(specular_light0_aux * numLevels) / numLevels;
		}else{
			specular_light0 = specular_light0_aux;
		}
	}

	float specular_light1 = 0;
	float specular_light1_aux = 0;

	if (diffuse_light1 > 0)
	{
		specular_light1_aux =  material_ks * pow(max(dot(world_normal, H1), 0), material_shininess);
		if(numLevels != 0){
			specular_light1 = floor(specular_light1_aux * numLevels) / numLevels;
		}else{
			specular_light1 = specular_light1_aux;
		}
	}

	float specular_light2 = 0;
	float specular_light2_aux = 0;

	if (diffuse_light2 > 0)
	{
		specular_light2_aux =  material_ks * pow(max(dot(world_normal, H2), 0), material_shininess);
		if(numLevels != 0){
			specular_light2 = floor(specular_light2_aux * numLevels) / numLevels;
		}else{
			specular_light2 = specular_light2_aux;
		}
	}

	float specular_light3 = 0;
	float specular_light3_aux = 0;

	if (diffuse_light3 > 0)
	{
		specular_light3_aux =  material_ks * pow(max(dot(world_normal, H3), 0), material_shininess);
		if(numLevels != 0){
			specular_light3 = floor(specular_light3_aux * numLevels) / numLevels;
		}else{
			specular_light3 = specular_light3_aux;
		}
	}

	//calculam factorii de atenuare si valoarea totala a intensitatii luminii pentru fiecare sursa de lumina
	float atenuare0 = 1.0f / (distance(light_position0, world_position) * distance(light_position0, world_position)) ;
	float light0 = ambient_light + atenuare0 * (diffuse_light0 + specular_light0);

	float atenuare1 = 1.0f / (distance(light_position1, world_position) * distance(light_position1, world_position)) ;
	float light1 = ambient_light + atenuare1 * (diffuse_light1 + specular_light1);

	float atenuare2 = 1.0f / (distance(light_position2, world_position) * distance(light_position2, world_position)) ;
	float light2 = ambient_light + atenuare2 * (diffuse_light2 + specular_light2);

	float atenuare3 = 1.0f / (distance(light_position3, world_position) * distance(light_position3, world_position)) ;
	float light3 = ambient_light + atenuare3 * (diffuse_light3 + specular_light3);

	//verificam daca avem de desenat mosul
	if(object_to_draw == 4){
		//extragem culorile difuza si speculara
		vec3 KD = texture2D(texture_1, texcoord).rgb;
		vec3 KS = texture2D(texture_2, texcoord).rgb;

		//calculam culoarea finala conform formulei din enunt
		vec3 aux = (ambient_light + diffuse_light0) * KD + specular_light0 * KS;
		vec3 sum0 = vec3(aux[0] * light_color0[0], aux[1] * light_color0[1], aux[2] * light_color0[2]) * atenuare0;

		aux = (ambient_light + diffuse_light1) * KD + specular_light1 * KS;
		vec3 sum1 = vec3(aux[0] * light_color1[0], aux[1] * light_color1[1], aux[2] * light_color1[2]) * atenuare1;

		aux = (ambient_light + diffuse_light2) * KD + specular_light2 * KS;
		vec3 sum2 = vec3(aux[0] * light_color2[0], aux[1] * light_color2[1], aux[2] * light_color2[2]) * atenuare2;

		aux = (ambient_light + diffuse_light3) * KD + specular_light3 * KS;
		vec3 sum3 = vec3(aux[0] * light_color3[0], aux[1] * light_color3[1], aux[2] * light_color3[2]) * atenuare3;

		vec3 sum = sum0 + sum1 + sum2 + sum3;
		out_color = vec4(sum, 1);
	}
	//verificam daca avem de desenat una dintre sursele de lumina
	else if(object_to_draw == 1){
		out_color = vec4(light1 * light_color1, 1);
	}
	else if(object_to_draw == 0){
		out_color = vec4(light0 * light_color0, 1);
	}
	else if(object_to_draw == 2){
		out_color = vec4(light2 * light_color2, 1);
	}
	else if(object_to_draw == 3){
		out_color = vec4(light3 * light_color3, 1);
	}else if (object_to_draw == 5){
		//pentru a desena planului folosesc o formula asemanatoare celei pentru santa, doar ca neavand culorile
		// difuza si speculara (KS si KD) ne vom folosi de culoarea luminilor si de culoarea obiectului (planului) 
		vec3 aux = (ambient_light + diffuse_light0 + specular_light0) * light_color0;
		vec3 sum0 = vec3(aux[0] * obj_color[0], aux[1] * obj_color[1], aux[2] * obj_color[2]) * atenuare0;

		aux = (ambient_light + diffuse_light1 + specular_light1) * light_color1;
		vec3 sum1 = vec3(aux[0] * obj_color[0], aux[1] * obj_color[1], aux[2] * obj_color[2]) * atenuare1;

		aux = (ambient_light + diffuse_light2 + specular_light2) * light_color2;
		vec3 sum2 = vec3(aux[0] * obj_color[0], aux[1] * obj_color[1], aux[2] * obj_color[2]) * atenuare2;

		aux = (ambient_light + diffuse_light3 + specular_light3) * light_color3;
		vec3 sum3 = vec3(aux[0] * obj_color[0], aux[1] * obj_color[1], aux[2] * obj_color[2]) * atenuare3;

		vec3 sum = sum0 + sum1 + sum2 + sum3;
		out_color = vec4(sum, 1);
	}else if (object_to_draw == 7){
		//santa pentru care se deseneaza doar fetele spate
		//are culoare constanta (negru)
		out_color = vec4(0,0,0,0);
	}
	else{
		//desenam fundalul (cerul) folosind doar lumina ambientala
		out_color = vec4(obj_color * ambient_light, 1);
	}
}