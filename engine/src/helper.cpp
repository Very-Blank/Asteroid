#include "helper.hpp"

namespace engine {
	void process_input(GLFWwindow *window){
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, true);
		}
	}

	void framebuffer_size_callback(GLFWwindow *window, int width, int height){
		glViewport(0, 0, width, height);
	}

	glm::vec2 rotate_vector(glm::vec2 vector, const float &rotation){
		float cos_theta = glm::cos(rotation);
		float sin_theta = glm::sin(rotation);
		return glm::vec2(
			vector.x * cos_theta - vector.y * sin_theta,
			vector.x * sin_theta + vector.y * cos_theta
		);
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
}
