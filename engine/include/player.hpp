#pragma once
#include <rigidbody.hpp>

namespace engine {
	class Player : public Rigidbody{
		public:
		Player(int layer, char* texture_path);
		Player(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path);
		void Start();
		void Update(float delta_time, GLFWwindow *window);
		glm::vec3 Move_vector(GLFWwindow *window);
		glm::quat Look_rotation(GLFWwindow *window);
	};
}
