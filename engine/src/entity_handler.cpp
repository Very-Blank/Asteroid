#include "entity_handler.hpp"

namespace engine {

	void Entity_handler::Init(){
		for (Entity* &entity: entitys) {
			entity->entity_handler = this;
			entity->texture = Load_texture(entity->path);
		}
		for (Rigidbody* &rigidbody: rigidbodies) {
			rigidbody->texture = Load_texture(rigidbody->path);
			rigidbody->entity_handler = this;
		}
	}

	void Entity_handler::Instantiate(Entity *entity){
		entity->entity_handler = this;
		entity->texture = Load_texture(entity->path);
		entitys.push_back(entity);
	}

	void Entity_handler::Instantiate(Rigidbody *rigidbody){
		rigidbody->entity_handler = this;
		rigidbody->texture = Load_texture(rigidbody->path);
		rigidbodies.push_back(rigidbody);
	}

	void Entity_handler::Updates(float delta_time, GLFWwindow *window){
		for (Entity* &entity: entitys) {
			entity->Update(delta_time, window);
		}
	
		Update_collisions(delta_time);

		for (Rigidbody* &rigidbody: rigidbodies) {
			rigidbody->Update(delta_time, window);
		}
	}

	void Entity_handler::Draws(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO){
		for (Entity* &entity: entitys) {
			entity->Draw(view, projection, shader_program, VAO);
		}
	
		for (Rigidbody* &rigidbody: rigidbodies) {
			rigidbody->Draw(view, projection, shader_program, VAO);
		}
	}
	//this is called after draw!
	void Entity_handler::Delete_entitys(){
		int size = entitys.size();
		for (Entity *entity : entity_delete_queue) {
			for (int i = 0; i < size; i++) {
				if(entity == entitys[i]){
					entity->Delete_entity();
					entitys[i] = entitys[size-1];
					entitys.pop_back();
					size --;
					break;
				}
			}
			delete entity;
		}

		entity_delete_queue.clear();

		size = rigidbodies.size();
		for (Rigidbody *rigidbody : rigidbody_delete_queue) {
			for (int i = 0; i < size; i++) {
				if(rigidbody == rigidbodies[i]){
					rigidbody->Delete_entity();
					rigidbodies[i] = rigidbodies[size-1];
					rigidbodies.pop_back();
					size--;
					break;
				}
			}
		}

		rigidbody_delete_queue.clear();
	}

	void Entity_handler::Despawn_entity(Entity *entity){
		entity_delete_queue.push_back(entity);
	}

	void Entity_handler::Despawn_entity(Rigidbody *rigidbody){
		rigidbody_delete_queue.push_back(rigidbody);
	}

	void Entity_handler::Update_collisions(float delta_time){
		int rigidbodies_size = rigidbodies.size();
		for(int i = 0; i < rigidbodies_size; i++){
			int j = i + 1;
			while(j < rigidbodies_size){
				if(Is_colliding(*rigidbodies[i], *rigidbodies[j])){
					On_collision(*rigidbodies[i], *rigidbodies[j], delta_time);
				}
				j++;
			}
		}
	}

	bool Entity_handler::Is_colliding(Rigidbody &body1, Rigidbody &body2){
		bool body_can_collide = false;
		for (int &mask : body1.mask) {
			if (mask == body2.layer) {
				body_can_collide = true;
				break;
			}
		}

		if (!body_can_collide) {
			return false;
		}

		body_can_collide = false;
		for (int &mask : body2.mask) {
			if (mask == body1.layer) {
				body_can_collide = true;
				break;
			}
		}

		if (!body_can_collide) {
			return false;
		}

		glm::vec2 body2_point_rot = rotate_vector(body2.collider_scale, body2.rotation - body1.rotation);
		glm::vec2 body2_point_rot2 = rotate_vector(body2.collider_scale * glm::vec2(-1.0f, 1.0f), body2.rotation - body1.rotation);
		glm::vec2 body2_local_offset = rotate_vector(body2.position - body1.position, -body1.rotation);

		glm::vec2 body1_point_rot = rotate_vector(body1.collider_scale, body1.rotation - body2.rotation);
		glm::vec2 body1_point_rot2 = rotate_vector(body1.collider_scale * glm::vec2(-1.0f, 1.0f), body1.rotation - body2.rotation);
		glm::vec2 body1_local_offset = rotate_vector(body1.position - body2.position, -body2.rotation);

		if(Is_inside_bounding_box(body1.collider_scale, body2_point_rot + body2_local_offset) ||
			Is_inside_bounding_box(body1.collider_scale, body2_point_rot2 + body2_local_offset) ||
			Is_inside_bounding_box(body1.collider_scale, body2_point_rot * glm::vec2(-1.0f, -1.0f) + body2_local_offset) ||
			Is_inside_bounding_box(body1.collider_scale, body2_point_rot2 * glm::vec2(-1.0f, -1.0f) + body2_local_offset)){
			return true;
		}
		else if(Is_inside_bounding_box(body2.collider_scale, body1_point_rot + body1_local_offset) ||
			Is_inside_bounding_box(body2.collider_scale, body1_point_rot2 + body1_local_offset) ||
			Is_inside_bounding_box(body2.collider_scale, body1_point_rot * glm::vec2(-1.0f, -1.0f) + body1_local_offset) ||
			Is_inside_bounding_box(body2.collider_scale, body1_point_rot2 * glm::vec2(-1.0f, -1.0f) + body1_local_offset)){
			return true;
		}

		return false;
	}

	bool Entity_handler::Is_inside_bounding_box(glm::vec2 collider, glm::vec2 point){
		// collision x-axis?
		bool collision_x = collider.x >= point.x && -collider.x <= point.x;
		// collision y-axis?
		bool collision_y = collider.y >= point.y && -collider.y <= point.y;
		// collision only if on both axes
		return collision_x && collision_y;
	}

	void Entity_handler::On_collision(Rigidbody &body1, Rigidbody &body2, float delta_time){
		float velocity_dir1 = glm::dot(glm::normalize(body1.velocity), glm::normalize(body2.position - body1.position));
		float velocity_dir2 = glm::dot(glm::normalize(body2.velocity), glm::normalize(body1.position - body2.position));

		if (velocity_dir1 >= 0.0f || velocity_dir2 >= 0.0f) {
			glm::vec3 new_velocity(0.0f);
			new_velocity.x = (body1.mass * body1.velocity.x + body2.mass * body2.velocity.x)/(body1.mass + body2.mass);
			new_velocity.y = (body1.mass * body1.velocity.y + body2.mass * body2.velocity.y)/(body1.mass + body2.mass);
			body1.velocity = new_velocity;
			body2.velocity = new_velocity;
		}

		body2.On_hit(&body1);
		body1.On_hit(&body2);
	}

	unsigned int Entity_handler::Load_texture(char* path){
			for (Texture &texture : textures) {
				if (texture.path.compare(path) == 0) {
					return texture.texture;
				}
			}

			unsigned int size = textures.size();
			textures.push_back(Texture());
			textures[size].path = path;

			glGenTextures(1, &textures[size].texture);
			glBindTexture(GL_TEXTURE_2D, textures[size].texture);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
			//loading image
			int width, height, nrChannels;
			//rotates the image correctly for opengl
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "Loaded the texture, path: " << path << std::endl;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(data);
			}
			else {
				std::cout << "Failed to load the texture, path: " << path << std::endl;
			}

			return textures[size].texture;
	}
}
