#pragma once

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
	class Entity{
		public:
		glm::vec3 position, scale;
		glm::quat rotation;
		glm::mat4 model;

		unsigned int texture;

		Entity(int layer, char* texture_path);
		Entity(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path);
		Entity(glm::vec2 _position, int layer, glm::vec2 _scale, glm::quat rotation, char* texture_path);
		
		void Delete_entity();
		void On_despawn();
		void Start();
		void Update(float delta_time);

		void Draw(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO);
		void Calculate_model_matrix();
		void Load_texture(char * texture_path);
		float Layer_to_z(int &layer);
	};
}
