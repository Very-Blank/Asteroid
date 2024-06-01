#include <cmath>
#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

//our header files
#include "asteroid.hpp"
#include "entity_handler.hpp"
#include "entity.hpp"
#include "helper.hpp"
#include "player.hpp"


int main(int argc, char const *argv[]){
	engine::Window_settings window_settings;
	window_settings.width = 800;
	window_settings.height = 800;

	enum Mode {normal, debug, lit_face_debug, line_debug};
	Mode mode = Mode::normal;

	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			mode = Mode::debug;
			std::cout << "debug mode active" << std::endl;
		}
		else if (strcmp(argv[i], "-dl") == 0){
			mode = Mode::line_debug; std::cout << "line debug mode active" << std::endl;
		}
		else if (strcmp(argv[i], "-dlf") == 0){
			mode = Mode::lit_face_debug;
			std::cout << "lit face debug mode active" << std::endl;
		}
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(window_settings.height, window_settings.width, window_settings.name, NULL, NULL);
	glfwSetWindowUserPointer(window, &window_settings);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (glfwRawMouseMotionSupported()){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glViewport(0, 0, 800, 800);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, engine::framebuffer_size_callback);

	//cube shaders
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	char* source = engine::load_txt_file("../../shaders/vertex.glsl");
	if (source == nullptr) {
		glfwTerminate();
		return -1;
	}

	glShaderSource(vertex_shader, 1, &source, NULL);
	glCompileShader(vertex_shader);

	int compile_status;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		std::cout << "failed to compile vertex shader" << std::endl;
		char info[512];
		glGetShaderInfoLog(vertex_shader, 512, NULL, info);
		std::cout << "failed to compile fragment shader" << std::endl;
		std::cout << "info: " << info << std::endl;
	}

	delete[] source;
	source = nullptr;

	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	source = engine::load_txt_file("../../shaders/fragment.glsl");

	if (source == nullptr) {
		glfwTerminate();
		return -1;
	}

	glShaderSource(fragment_shader, 1, &source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		std::cout << "failed to compile fragment shader" << std::endl;
		char info[512];
		glGetShaderInfoLog(fragment_shader, 512, NULL, info);
		std::cout << "info: " << info << std::endl;
	}

	delete[] source;
	source = nullptr;

	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	glGetProgramiv(shader_program, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		std::cout << "failed to link" << std::endl;
	}

	float vertices[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	};

	float collision_vertices[] = {
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 2,
	};
	
	//buffers for face
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//We bind the vertex array so it starts to "record" what VBO to use and what EBO to use.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	//this stops the VAO from "recording"
	glBindVertexArray(0);

	if(mode == Mode::line_debug)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	char player_path[] = "../../textures/ship.png";
	char asteroid_path[] = "../../textures/asteroid.png";

	engine::Entity_handler entity_handler;

	glm::vec2 positions[] = {
		glm::vec2(50.0f, 0.0f),
		glm::vec2(-100.0f, -200.0f),
		glm::vec2(100.0f, 50.0f),
		glm::vec2(120.0f, -80.0f),
		glm::vec2(170.0f, 300),
		glm::vec2(-80.0f, 150.0f),
		glm::vec2(200.0f, -400.0f),
		glm::vec2(-200.0f, 100.0f),
		glm::vec2(300.0f, 300.0f),
		glm::vec2(-300.0f, 500.0f),
		glm::vec2(400.0f, 100.0f),
		glm::vec2(-450.0f, 200.0f),
	};

	float collider_scale = 40.0f;
	for (glm::vec2 &position : positions) {
		engine::Asteroid *asteroid = new engine::Asteroid(position, 2.0f, glm::vec2(50.0f), asteroid_path, GL_TEXTURE0);
		asteroid->mass = 1.0f;
		asteroid->health = 20.0f;
		asteroid->name = "asteroid";
		asteroid->collider_scale = glm::vec2(collider_scale);
		entity_handler.rigidbodies.push_back(asteroid);
	}

	engine::Player *player = new engine::Player(glm::vec2(-50.0f), 1.0f, glm::vec2(50.0f), player_path, GL_TEXTURE0);
	player->mass = 1.0f;
	player->name = "player";
	player->collider_scale = glm::vec2(50.0f, 50.0f);
	entity_handler.player = player;
	entity_handler.rigidbodies.push_back(player);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(player->position.x, player->position.y, player->layer));
	// glm::mat4 projection =  glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	window_settings.projection = glm::ortho(-window_settings.width/2.0f, window_settings.width/2.0f, -window_settings.height/2.0f, window_settings.height/2.0f, 0.1f, 100.0f);

	glEnable(GL_BLEND);

	float delta_time = 0.0f;
	float last_frame = 0.0f;

	entity_handler.Init();

	while (!glfwWindowShouldClose(window)) {
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		engine::process_input(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		entity_handler.Updates(delta_time, window);
		entity_handler.Update_collisions(delta_time);

		glm::vec3 cam_pos = -glm::vec3(player->position.x, player->position.y, player->layer);
		cam_pos.z = -1.0f;
		view = glm::mat4(1.0f);
		view = glm::translate(view, cam_pos);

		entity_handler.Draws(view, window_settings.projection, shader_program, VAO);
		entity_handler.Delete_entitys();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);

	glfwTerminate();

	return 0;
}
