#include <player.hpp>
#include "bullet.hpp"
#include "entity_handler.hpp"

namespace engine {
	Player::Player(int layer, char* texture_path, int _texture_id) : Rigidbody(layer, texture_path, _texture_id){
		is_on_cooldown = false;
		max_cooldown = 0.5f;
	}
	Player::Player(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id) : Rigidbody(_position, layer, _scale, texture_path, _texture_id){
		is_on_cooldown = false;
		max_cooldown = 0.5f;
	}

	void Player::Update(float delta_time, GLFWwindow *window){
		if (is_on_cooldown) {
			cooldown += delta_time;
			if (max_cooldown <= cooldown) {
				is_on_cooldown = false;
				cooldown = 0.0f;
			}
		}

		rotation = Look_rotation(window);
		velocity += Move_vector(window) * 50.0f;
		if(glm::length(velocity) > 400.0f){
			velocity = glm::normalize(velocity) * 400.0f;
		}

		if (Just_pressed_mouse(window) && !is_on_cooldown) {
			char asteroid_path[] = "../../textures/bullet.png";
			glm::vec2 direction = rotate_vector(glm::vec2(0.0f, 1.0f), rotation);
			Bullet *bullet = new Bullet(position + direction * 80.0f, 1.0f, glm::vec2(20.0f), asteroid_path, GL_TEXTURE0);
			bullet->name = "bullet";
			bullet->damage = 10.0f;
			bullet->rotation = rotation;
			bullet->mass = 1.0f;
			bullet->collider_scale = glm::vec2(20.0f);
			bullet->velocity = direction * 800.0f + velocity;
			entity_handler->Instantiate(bullet);
			is_on_cooldown = true;
		}

		position.x += velocity.x * delta_time;
		position.y += velocity.y * delta_time;
		Calculate_model_matrix();
	}

	bool Player::Just_pressed_mouse(GLFWwindow *window){
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mouse_down) {
			mouse_down = true;
			return true;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			mouse_down = false;
		}
		return false;
	}

	void Player::Start(){
		max_cooldown = 0.5f;
	}

	glm::vec2 Player::Move_vector(GLFWwindow *window){
		glm::vec2 move_vector(0.0f);

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

	float Player::Look_rotation(GLFWwindow* window){
		Window_settings* window_settings = static_cast<Window_settings*>(glfwGetWindowUserPointer(window));

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		glm::vec2 mouse_direction = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)) - glm::vec2(window_settings->width/2.0f, window_settings->height/2.0f);
		
		mouse_direction = glm::normalize(mouse_direction);
		float rotation = 0.0f;
		float dot_up = glm::dot(glm::vec2(0.0f, 1.0f), mouse_direction);
		float dot_right = glm::dot(glm::vec2(1.0f, 0.0f), mouse_direction);
		
		if(dot_right < 0.0f)
			rotation = glm::asin(dot_up) + std::numbers::pi/2.0f;
		else
			rotation = -glm::asin(dot_up) - std::numbers::pi/2.0f;

		return rotation;
	}
}
