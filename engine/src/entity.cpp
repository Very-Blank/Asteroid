#include "entity.hpp"
#include "entity_handler.hpp"

namespace engine {
		Entity::Entity(float _layer, char* texture_path, int _texture_id) : texture_id(_texture_id), layer(_layer){
			path = texture_path;
			type = CLASS_TYPE::ENTITY;
			position = glm::vec2(0.0f);
			scale = glm::vec2(50.0f);
			rotation = 0.0f;
			Calculate_model_matrix();
		}

		Entity::Entity(glm::vec2 _position, float _layer, glm::vec2 _scale, char* texture_path, int _texture_id) : position(_position), layer(_layer), scale(_scale), texture_id(_texture_id){
			path = texture_path;
			type = CLASS_TYPE::ENTITY;
			rotation = 0.0f;
			Calculate_model_matrix();
		}

		Entity::Entity(glm::vec2 _position, float _layer, glm::vec2 _scale, float rotation, char* texture_path, int _texture_id) : position(_position), layer(_layer), scale(_scale), rotation(rotation), texture_id(_texture_id){
			path = texture_path;
			type = CLASS_TYPE::ENTITY;
			Calculate_model_matrix();
		}
		
		void Entity::Start(){
		}

		void Entity::Update(float delta_time, GLFWwindow *window){
		}

		void Entity::Delete_entity(){
		}

		void Entity::Draw(glm::mat4 &view, glm::mat4 &projection, unsigned int &shader_program, unsigned int &VAO){
			glUseProgram(shader_program);
	
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform1i(glGetUniformLocation(shader_program, "a_texture"), 0);

			glActiveTexture(texture_id);
			glBindTexture(GL_TEXTURE_2D, texture);
		
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}


		void Entity::Calculate_model_matrix(){
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(position.x, position.y, -layer));  // Translate
			model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));        // Rotate
			model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));             // Scale
		}
}
