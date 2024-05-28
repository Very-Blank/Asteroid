#include "rigidbody.hpp"

namespace engine {
	class Bullet : public Rigidbody{
		public:
		float damage;
		Bullet(glm::vec2 _position, float layer, glm::vec2 _scale, char* texture_path, int _texture_id);
		void On_hit(Rigidbody *other_body) override;
	};
}
