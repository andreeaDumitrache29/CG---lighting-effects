#include "Laborator9.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator9::Laborator9()
{
}

Laborator9::~Laborator9()
{
}

vector <glm::vec3> lightPosition;
vector <glm::vec3> lightColor;

void Laborator9::Init()
{
	//boolean pentru a activa / dezactiva miscarea luminilor
	lights_move = false;
	//boolean pentru a activa / dezactiva efectul de tooning
	activate_tooning = false;

	// angularStep si velocity folosite pentru a calcula unghiul curent necesar miscarii luminilor
	angularStep = 0.0f;
	velocity = 2.5f;
	//numarul de nivele pentru efectul de stepped lighting
	numLevels = 3.0f;

	{
		//pozitiile luminilor
		lightPosition.push_back(glm::vec3(0.0f, 2.15f, -1.0f));
		lightPosition.push_back(glm::vec3(0.0f, 2.15f, 1.0f));
		lightPosition.push_back(glm::vec3(1, 2.15f, 0.0f));
		lightPosition.push_back(glm::vec3(-1, 2.15f, 0.0f));

		//culorile luminilor
		lightColor.push_back(glm::vec3(0.1f, 1.0f, 0.8f));
		lightColor.push_back(glm::vec3(0.8f, 0.7f, 1.0f));
		lightColor.push_back(glm::vec3(0.8f, 0.8f, 0.0f));
		lightColor.push_back(glm::vec3(1.0f, 0.2f, 0.2f));

		//proprietatile de material
		materialShininess = 100;
		materialKd = 0.9;
		materialKs = 0.9;
	}

	const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";

	// Incarcam texturile
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "santa_diffuse.png").c_str(), GL_REPEAT);
		mapTextures["santa_d"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "santa_specular.png").c_str(), GL_REPEAT);
		mapTextures["santa_s"] = texture;
	}

	//incarcam primitivele folosite: sfera, plan si santa

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("santa");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "santa.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Laborator9::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Laborator9::Update(float deltaTimeSeconds)
{
	clock = Engine::GetElapsedTime();

	//calculam noile pozitii ale surselor luminilor daca miscarea lor este activa
	if (lights_move == true) {
		angularStep += deltaTimeSeconds * velocity;
		//primele doua lumini descriu curbe Lissajous 
		lightPosition.at(0).x = 6 * sin(angularStep * 3) / 3.0;
		lightPosition.at(0).z = 6 * sin(4 * angularStep) / 3.0;
		lightPosition.at(1).x = 5 * sin(angularStep * 3) / 3.0;
		lightPosition.at(1).z = 5 * sin(2 * angularStep) / 3.0;
		//a treia lumina descrie o curba Talbot
		float a = 1.4; float b = 0.5;
		float c = sqrt(a*a - b*b);
		float e = c / a;
		lightPosition.at(2).x = a * cos(angularStep) * (1 + e * e * sin(angularStep) * sin(angularStep));
		lightPosition.at(2).z = (a * sin(angularStep) * (1 - 2 * e*e + e*e*sin(angularStep)*sin(angularStep))) / sqrt(1 - e*e);
		//a patra lumina descrie o curba Plateau
		float m = 2.0; float n = 0.8;
		lightPosition.at(3).x = (a / 3.0) * sin((m + n) * angularStep) / sin((m - n) * angularStep);
		lightPosition.at(3).z = 2 * (a / 3.0) * sin(m * angularStep) * sin(n * angularStep) / sin((m - n) * angularStep);
	} 

	//prima sursa de lumina
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition.at(0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 0, NULL, NULL);
	}

	//a doua sursa de lumina
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition.at(1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 1, NULL, NULL);
	}

	//a treia sursa de lumina
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition.at(2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 2, NULL, NULL);
	}

	//a patra sursa de lumina
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition.at(3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 3, NULL, NULL);
	}

	//planul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(50.0f));
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix, 5, NULL, NULL);
	}

	//santa
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.010f));
		RenderSimpleMesh(meshes["santa"], shaders["ShaderLab9"], modelMatrix, 4, mapTextures["santa_d"], mapTextures["santa_s"]);
	}

	//activam desenarea fetelor spate pentru al doilea santa, necesar pentru desenarea conturului
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.010f));
		RenderSimpleMesh(meshes["santa"], shaders["ShaderLab9"], modelMatrix, 7, mapTextures["santa_d"], mapTextures["santa_s"]);
	}
	glDisable(GL_CULL_FACE);

	//desenam cerul
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 6, NULL, NULL);
	}
}

void Laborator9::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int obj, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//ceasul sistemului
	int location_clock = glGetUniformLocation(shader->program, "Clock");
	glUniform1f(location_clock, clock);

	/** verificam daca efectul de tooning este activat
	daca da, verificam daca obiectul de desenat este al doilea santa
	daca da, atunci informam vertex shader-ul prin uniforma tooning, pentru a calcula corespunzator
	noile pozitii ale vertecsilor **/
	int location_toon = glGetUniformLocation(shader->program, "tooning");
	if (obj == 7) {
		if (activate_tooning == true) {
			glUniform1i(location_toon, 1);
		}
		else {
			glUniform1i(location_toon, 0);
		}
	}
	else {
		glUniform1i(location_toon, 0);
	}

	// trimitem prima textura
	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	//trimitem a doua textura
	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	//trimitem pozitiile luminior catre fragment shader
	int light_position = glGetUniformLocation(shader->program, "light_position0");
	glUniform3f(light_position, lightPosition.at(0).x, lightPosition.at(0).y, lightPosition.at(0).z);

	light_position = glGetUniformLocation(shader->program, "light_position1");
	glUniform3f(light_position, lightPosition.at(1).x, lightPosition.at(1).y, lightPosition.at(1).z);

	light_position = glGetUniformLocation(shader->program, "light_position2");
	glUniform3f(light_position, lightPosition.at(2).x, lightPosition.at(2).y, lightPosition.at(2).z);

	light_position = glGetUniformLocation(shader->program, "light_position3");
	glUniform3f(light_position, lightPosition.at(3).x, lightPosition.at(3).y, lightPosition.at(3).z);

	//trimitem culorile luminilor catre fragment shader
	int light_color = glGetUniformLocation(shader->program, "light_color0");
	glUniform3f(light_color, lightColor.at(0).x, lightColor.at(0).y, lightColor.at(0).z);

	int light_color1 = glGetUniformLocation(shader->program, "light_color1");
	glUniform3f(light_color1, lightColor.at(1).x, lightColor.at(1).y, lightColor.at(1).z);

	int light_color2 = glGetUniformLocation(shader->program, "light_color2");
	glUniform3f(light_color2, lightColor.at(2).x, lightColor.at(2).y, lightColor.at(2).z);

	int light_color3 = glGetUniformLocation(shader->program, "light_color3");
	glUniform3f(light_color3, lightColor.at(3).x, lightColor.at(3).y, lightColor.at(3).z);

	// trimitem pozitia camerei
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	//trimitem proprietatile de material
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	//uniforma pentru a descrie ce obiect este cel curent, pentru a aplica
	//formulele corespunzatoare in fragment shader
	int obj_loc = glGetUniformLocation(shader->program, "object_to_draw");
	glUniform1i(obj_loc, obj);

	//numarul de nivele pentru efectul de stepped lighting
	int loc_tooning = glGetUniformLocation(shader->program, "numLevels");
	
	if (!activate_tooning) {
		glUniform1f(loc_tooning, 1);
	}
	else {
		glUniform1f(loc_tooning, numLevels);
	}
	

	//daca avem de desenat planul sau cerul trimitem si o culoare pentru fiecare
	if (obj == 5) {
		int obj_loc = glGetUniformLocation(shader->program, "obj_color");
		glUniform3f(obj_loc, 1.0f, 1.0f, 1.0f);
	}

	if (obj == 6) {
		int obj_loc = glGetUniformLocation(shader->program, "obj_color");
		glUniform3f(obj_loc, 0.0f, 0.20f, 0.70f);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator9::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}

	if (activate_tooning) {
		//creste numLevels
		if (window->KeyHold(GLFW_KEY_M)) {
			numLevels += 0.15f;
		}

		//scade numLevels
		if (window->KeyHold(GLFW_KEY_N)) {
			numLevels -= 0.15f;
		}
	}

}

void Laborator9::OnKeyPress(int key, int mods)
{
	//activeaza / dezactiveaza miscarea luminilor
	if (window->KeyHold(GLFW_KEY_L)) {
		lights_move = !lights_move;
	}

	//activeaza / dezactiveaza efectul de tooning
	if (window->KeyHold(GLFW_KEY_T)) {
		activate_tooning = !activate_tooning;
	}

}

void Laborator9::OnKeyRelease(int key, int mods)
{
	//activeaza / dezactiveaza miscarea luminilor
	if (window->KeyHold(GLFW_KEY_L)) {
		lights_move = !lights_move;
	}

	//activeaza / dezactiveaza efectul de tooning
	if (window->KeyHold(GLFW_KEY_T)) {
		activate_tooning = !activate_tooning;
	}
}

void Laborator9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator9::OnWindowResize(int width, int height)
{
}
