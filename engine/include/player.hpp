#pragma once
#include <rigidbody.hpp>

namespace engine {
	class Player : public Rigidbody{
		public:
		bool mouse_down, is_on_cooldown;
		float cooldown, max_cooldown;
		Player(int layer, char* texture_path, int _texture_id);
		Player(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id);
		void Start() override;
		void Update(float delta_time, GLFWwindow *window) override;
		bool Just_pressed_mouse(GLFWwindow *window);
		glm::vec2 Move_vector(GLFWwindow *window);
		float Look_rotation(GLFWwindow *window);
	};
}
