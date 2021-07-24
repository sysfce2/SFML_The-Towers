#pragma once

#include "Entity.hpp"
#include "Camera.hpp"
#include <vector>
#include <unordered_map>
#include <SFML/Network.hpp>
#include "NetworkManager.hpp"
#include "Cube.hpp"
#include "Shader.hpp"
#include "Assets.hpp"

class EntityHandler {
public:


	void init(Assets* _assets);
	void update(float _deltaTime);
	void render(Camera& camera);
	void destroy();

	void updateEntity(uint8_t id, const math::vec3& position, float pitch, float yaw);
	void removeEntity(uint8_t entity);


private:

	void addEntity(uint8_t id, const math::vec3& position, float pitch, float yaw);

	//Data Structures
	std::unordered_map<uint8_t, Entity> m_entities;

	Assets* m_assets = nullptr;
	Shader m_shader;

};
