#include "bullet.hpp"
#include "entity_handler.hpp"

namespace engine {
	Bullet::Bullet(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id) : Rigidbody(_position, layer, _scale, texture_path, _texture_id){
		damage = 0.0f;
	}
	void Bullet::On_hit(Rigidbody *other_body){
		entity_handler->Despawn_entity(this);

		other_body->health -= damage;
		if (other_body->health <= 0.0f) {
			other_body->On_destory();
		}
	}
}
