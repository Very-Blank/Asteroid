#include <cmath>
#include <fstream>
#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

//our header files
#include <entity.hpp>

bool Colliding(glm::vec3 first_position, glm::vec3 second_position, float vertices[]);
glm::vec3 move_vector(GLFWwindow *window);
glm::quat player_look_rotation(GLFWwindow* window);
char* load_txt_file(const char* file_path);
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;
const char* GAME_NAME = "Asteroid";

int main(int argc, char const *argv[])
{
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
	
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIDTH, GAME_NAME, NULL, NULL);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//cube shaders
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	char* source = load_txt_file("../../shaders/vertex.glsl");
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
	source = load_txt_file("../../shaders/fragment.glsl");

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

	unsigned int ship_texture;
	glGenTextures(1, &ship_texture);
	glBindTexture(GL_TEXTURE_2D, ship_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//loading image
	int width, height, nrChannels;
	//rotates the image correctly for opengl
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("../../textures/ship.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load the image" << std::endl;
	}

	glm::vec3 player_position(0.0f);
	glm::vec3 player_velocity(0.0f);
	player_position.z = 1.0f;
	glm::vec3 scale(0.1f);
	glm::quat player_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 player_model(1.0f);
	player_model = glm::scale(player_model, scale);
	player_model *= glm::toMat4(player_rotation);
	player_model = glm::translate(player_model, player_position);

	glm::mat4 view(1.0f);
	view = glm::translate(view, -player_position);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 1000.0f);
	char asteroid_path[] = "../../textures/asteroid.png";
	engine::Entity asteroid(1, asteroid_path);
	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		process_input(window);

		player_rotation = player_look_rotation(window);
		player_velocity += move_vector(window) * 0.005f;
		if(glm::length(player_velocity) > 0.07f){
			player_velocity = glm::normalize(player_velocity) * 0.07f;
		}

		player_position += player_velocity;
		player_model = glm::mat4(1.0f);
		player_model = glm::scale(player_model, scale);
		player_model = glm::translate(player_model, player_position);
		player_model *= glm::toMat4(player_rotation);

		view = glm::mat4(1.0f);
		glm::vec3 cam_pos = player_position;
		cam_pos.x = cam_pos.x * scale.x;
		cam_pos.y = cam_pos.y * scale.y;
		view = glm::translate(view, -cam_pos);

		if(Colliding(player_position, asteroid.position, collision_vertices)){
			player_velocity = glm::vec3(0.0f);
		}

		glUseProgram(shader_program);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(player_model));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ship_texture);
	
		glBindVertexArray(VAO);
		//no need to specify what EBO or VBO to use, VAO has it stored!
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		asteroid.Draw(view, projection, shader_program, VAO);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);
	glDeleteTextures(1, &ship_texture);

	glfwTerminate();

	return 0;
}

bool Colliding(glm::vec3 first_position, glm::vec3 second_position, float vertices[]){
	bool thing = false;
	glm::vec3 first_corner_1 = glm::vec3(vertices[0], vertices[1], vertices[2]) - first_position;
	glm::vec3 first_corner_3 = glm::vec3(vertices[6], vertices[7], vertices[8]) - first_position;
	glm::vec3 first_corner_4 = glm::vec3(vertices[9], vertices[10], vertices[11]) - first_position;

	for(int i = 0; i < 12; i += 3){
		glm::vec3 position = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]) - second_position;
		if(first_corner_4.y < position.y && position.y < first_corner_1.y && first_corner_4.x < position.x && position.x < first_corner_3.x){
			thing = true;
		}
	}

	return thing;
}

glm::vec3 move_vector(GLFWwindow *window){
	glm::vec3 move_vector(0.0f);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		move_vector.y += 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		move_vector.y -= 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		move_vector.x += 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		move_vector.x -= 1.0f;
	}

	return move_vector;
}

glm::quat player_look_rotation(GLFWwindow* window){
	glm::quat result;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glm::vec3 mouse_direction = glm::vec3(static_cast<float>(xpos), static_cast<float>(ypos), 0.0f) - glm::vec3(400.0f, 400.0f, 0.0f);
	
	mouse_direction = glm::normalize(mouse_direction);
	float rotation = 0.0f;
	float dot_up = glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), mouse_direction);
	float dot_right = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), mouse_direction);
	
	if(dot_right < 0.0f)
		rotation = glm::asin(dot_up) + std::numbers::pi/2.0f;
	else
		rotation = -glm::asin(dot_up) - std::numbers::pi/2.0f;

	return glm::quat(glm::vec3(0.0f, 0.0f, rotation));
}

char* load_txt_file(const char* file_path){
	std::ifstream file(file_path, std::ifstream::binary);
	if(!file){
		std::cout << "Problem with opening the file" << std::endl;
		std::cout << "File: " << file_path << std::endl;
		return nullptr;
	}

	file.seekg(0, file.end);
	size_t size = file.tellg();
	
	try{
		file.unget();
		char* contents;
		if(file.peek() == '\0'){
			file.seekg(0, file.beg);
			contents = new char[size];
			file.read(contents, size);
		}
		else {
			file.seekg(0, file.beg);
			contents = new char[size + 1];
			file.read(contents, size);
			contents[size] = '\0';
		}
			
		if (file) {
			std::cout << "File: " << file_path << " read successfully" << std::endl;
		}
		else {
			std::cout << "Reading the whole file failed only: " << file.gcount() << " where read" << std::endl;
			std::cout << "File: " << file_path << std::endl;
		}

		file.close();

		return contents;

	} catch (const std::bad_alloc& e){
		std::cerr << "Failed to allocate memory: " << e.what() << std::endl;
	}

	file.close();

	return nullptr;
}

void process_input(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}


