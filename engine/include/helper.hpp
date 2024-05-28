#pragma once
#include "rigidbody.hpp"
#include <fstream>
#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/common.hpp>

namespace engine {
	void process_input(GLFWwindow *window);
	bool Is_inside_bounding_box(glm::vec2 collider, glm::vec2 point);
	char* load_txt_file(const char* file_path);
	void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	//rotation is in radians
	glm::vec2 rotate_vector(glm::vec2 vector, const float &rotation);
}

