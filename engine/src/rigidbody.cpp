#include <rigidbody.hpp>
#include <vector>
#include "entity_handler.hpp"

namespace engine {

	Rigidbody::Rigidbody(float layer, char* texture_path, int _texture_id) : Entity(layer, texture_path, _texture_id), velocity(glm::vec3(0.0f)){
		this->layer = 0;
		mask = {0, 1};
		type = CLASS_TYPE::RIGIDBODY;
	}
	Rigidbody::Rigidbody(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id) : Entity(_position, layer, _scale, texture_path, _texture_id), velocity(0.0f){
		this->layer = 0;
		mask = {0, 1};
		type = CLASS_TYPE::RIGIDBODY;
	}

	void Rigidbody::Update(float delta_time, GLFWwindow *window){
		position.x += velocity.x * delta_time;
		position.y += velocity.y * delta_time;
		Calculate_model_matrix();
	}
	
	void Rigidbody::On_hit(Rigidbody *other_body){}

	void Rigidbody::On_destory(){
		entity_handler->Despawn_entity(this);
	}
}
