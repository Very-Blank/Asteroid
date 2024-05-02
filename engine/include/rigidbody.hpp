#pragma once
#include "entity.hpp"

namespace engine {
	class Rigidbody : public Entity {
		public:
		float mass;
		glm::vec3 collider_scale;
		// m/s
		glm::vec3 velocity;
		Rigidbody(int layer, char* texture_path);
		Rigidbody(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path);
		void On_Collision(Rigidbody body, bool force_applied);
		void Update(float delta_time);
	};
}
