#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "rigidbody.hpp"
namespace engine {
	class Asteroid : public Rigidbody{
		public:
		Asteroid(float layer, char* texture_path, int _texture_id);
		Asteroid(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id);
		int debris_count;
		void On_destory() override;
	};
}

#endif
