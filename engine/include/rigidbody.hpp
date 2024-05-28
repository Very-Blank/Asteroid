#pragma once
#include "entity.hpp"
#include <vector>

namespace engine {
	class Rigidbody : public Entity {
		public:
		float mass, health, layer;
		std::vector<int> mask;
		bool bounce;
		//velocity is in m/s
		glm::vec2 collider_scale, velocity;
		Rigidbody(float layer, char* texture_path, int _texture_id);
		Rigidbody(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id);
		void Update(float delta_time, GLFWwindow *window) override;
		virtual void On_hit(Rigidbody *other_body);
		virtual void On_destory();
	};
}
