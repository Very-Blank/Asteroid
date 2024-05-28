#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include <stb_image.h>


namespace engine {
	class Entity_handler;

	enum CLASS_TYPE{
		ENTITY,
		RIGIDBODY,
	};

	class Entity{
		public:
		CLASS_TYPE type;
		std::string name;

		glm::vec2 position, scale;
		float health, layer, rotation;
		glm::mat4 model;

		unsigned int texture;
		char* path;
		int texture_id;

		Entity_handler *entity_handler;

		Entity(float _layer, char* texture_path, int _texture_id);
		Entity(glm::vec2 _position, float _layer, glm::vec2 _scale, char* texture_path, int _texture_id);
		Entity(glm::vec2 _position, float _layer, glm::vec2 _scale, float rotation, char* texture_path, int _texture_id);
		
		void Delete_entity();
		virtual void Start();
		virtual void Update(float delta_time, GLFWwindow *window);

		void Draw(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO);
		void Calculate_model_matrix();
	};
}

#endif
