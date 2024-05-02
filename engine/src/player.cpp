#include <player.hpp>


namespace engine {
	Player::Player(int layer, char* texture_path) : Rigidbody(layer, texture_path){}
	Player::Player(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path) : Rigidbody(_position, layer, _scale, texture_path){}

	void Rigidbody::On_Collision(Rigidbody body, bool force_applied){
		if (!force_applied) {
			float new_velocity = (mass * velocity.length() + body.mass * body.velocity.length() * glm::dot(velocity, body.velocity))/(mass + body.mass);
			velocity = glm::normalize(velocity) * new_velocity;
			Calculate_model_matrix();
		}
	}

	void Player::Update(float delta_time, GLFWwindow *window){
		rotation = Look_rotation(window);
		velocity += Move_vector(window) * 0.005f;
		if(glm::length(velocity) > 0.07f){
			velocity = glm::normalize(velocity) * 0.07f;
		}

		position += velocity;
		Calculate_model_matrix();
	}

	glm::vec3 Player::Move_vector(GLFWwindow *window){
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

	glm::quat Look_rotation(GLFWwindow* window){
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
}
