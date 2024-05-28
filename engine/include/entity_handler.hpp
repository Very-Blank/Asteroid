#ifndef ENTITY_HANLDER_HPP
#define ENTITY_HANLDER_HPP

#include "entity.hpp"
#include "helper.hpp"
#include "player.hpp"
#include "rigidbody.hpp"

namespace engine {
	struct Texture{
		std::string path;
		unsigned int texture;
	};

	class Entity_handler{
		private:
		std::vector<Rigidbody*> rigidbody_delete_queue;
		std::vector<Entity*> entity_delete_queue;

		public:
		Player* player;
		std::vector<Texture> textures;
		std::vector<Rigidbody*> rigidbodies;
		std::vector<Entity*> entitys;


		void Init();
		void Instantiate(Entity *entity);
		void Instantiate(Rigidbody *rigidbody);
		void Updates(float delta_time, GLFWwindow *window);
		void Draws(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO);
		void Despawn_entity(Entity *entity);
		void Despawn_entity(Rigidbody *rigidbody);

		//this is called after draw!
		void Delete_entitys();

		void Update_collisions(float delta_time);
		bool Is_colliding(Rigidbody &body1, Rigidbody &body2);
		void On_collision(Rigidbody &body1, Rigidbody &body2, float delta_time);
		bool Is_inside_bounding_box(glm::vec2 collider, glm::vec2 point);
		
		unsigned int Load_texture(char* path);
	};
}
#endif
