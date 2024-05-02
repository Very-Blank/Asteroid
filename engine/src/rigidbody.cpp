#include <rigidbody.hpp>

namespace engine {

	Rigidbody::Rigidbody(int layer, char* texture_path) : Entity(layer, texture_path){}
	Rigidbody::Rigidbody(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path) : Entity(_position, layer, _scale, texture_path){}
	void Rigidbody::On_Collision(Rigidbody body, bool force_applied){
		if (!force_applied) {
			float new_velocity = (mass * velocity.length() + body.mass * body.velocity.length() * glm::dot(velocity, body.velocity))/(mass + body.mass);
			velocity = glm::normalize(velocity) * new_velocity;
			Calculate_model_matrix();
		}
	}
	void Rigidbody::Update(float delta_time){
		position += velocity * delta_time;
	}
}
