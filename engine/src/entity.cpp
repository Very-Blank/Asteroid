#include <entity.hpp>

namespace engine {
		Entity::Entity(int layer, char* texture_path){
			Load_texture(texture_path);
			position = glm::vec3(0.0f, 0.0f, Layer_to_z(layer));
			scale = glm::vec3(0.1f);
			rotation = glm::quat(glm::vec3(0.0f));
			Calculate_model_matrix();
			Start();
		}

		Entity::Entity(glm::vec2 _position, int layer, glm::vec2 _scale, char* texture_path){
			Load_texture(texture_path);
			position = glm::vec3(_position.x, _position.y, Layer_to_z(layer));
			scale = glm::vec3(_scale.x, _scale.y, 1.0f);
			rotation = glm::quat(glm::vec3(0.0f));
			Calculate_model_matrix();
			Start();
		}

		Entity::Entity(glm::vec2 _position, int layer, glm::vec2 _scale, glm::quat rotation, char* texture_path) : rotation(rotation){
			Load_texture(texture_path);
			position = glm::vec3(_position.x, _position.y, Layer_to_z(layer));
			scale = glm::vec3(_scale.x, _scale.y, 1.0f);
			Calculate_model_matrix();
			Start();
		}
		
		void Entity::Start(){
		}

		void Entity::Update(float delta_time){
		}

		void Entity::Delete_entity(){
			glDeleteTextures(1, &texture);
		}

		void Entity::On_despawn(){
		}

		void Entity::Draw(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO){
			glUseProgram(shader_program);
	
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
		
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		float Entity::Layer_to_z(int &layer){
			return static_cast<float>(layer)/10.0f;
		}

		void Entity::Calculate_model_matrix(){
			model = glm::mat4(1.0f);
			model = glm::scale(model, scale);
			model = glm::translate(model, position);
			model *= glm::toMat4(rotation);
		}

		void Entity::Load_texture(char * texture_path){
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
			//loading image
			int width, height, nrChannels;
			//rotates the image correctly for opengl
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(data);
			}
			else {
				std::cout << "Failed to load the image" << std::endl;
			}
		}
}
