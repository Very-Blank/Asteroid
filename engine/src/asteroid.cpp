#include "asteroid.hpp"
#include "entity_handler.hpp"

namespace engine {
	Asteroid::Asteroid(float layer, char* texture_path, int _texture_id) : Rigidbody(layer, texture_path, _texture_id){
		debris_count = 4;
	}
	Asteroid::Asteroid(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id) : Rigidbody(_position, layer, _scale, texture_path, _texture_id){
		debris_count = 4;
	}

	void Asteroid::On_destory(){
		char asteroid_path[] = "../../textures/broken_piece.png";
		for (int i = 0; i < debris_count; i++) {
			float rotation = std::numbers::pi/2.0f * i;
			engine::Rigidbody *piece = new engine::Rigidbody(position, 2.0f, glm::vec2(50.0f/2.0f), asteroid_path, GL_TEXTURE0);
			piece->rotation = rotation;
			piece->velocity = rotate_vector(glm::vec2(0.0f, 1.0f), rotation) * 100.0f;
			piece->layer = 1;
			piece->mask = {0};
			piece->mass = 0.5f;
			piece->health = 20.0f;
			piece->name = "piece";
			piece->collider_scale = glm::vec2(50.0f/4.0f);
			entity_handler->Instantiate(piece);
		}

		entity_handler->Despawn_entity(this);
	}
}
