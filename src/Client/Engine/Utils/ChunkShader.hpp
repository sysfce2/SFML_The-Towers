#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include "Utils.hpp"
#include "Shader.hpp"


class ChunkShader : public Shader {
public:

	void init();
	void loadProjectionMatrix(const glm::mat4& matrix);
	void loadViewMatrix(const glm::mat4& matrix);
	void loadGradient(float gradient);
	void loadDensity(float density);
	void loadChunkPosition(int _x, int _y, int _z);
	void loadCameraPosition(const glm::vec3& position);


private:

	void getUniformLocations();

	GLint m_viewMatrixLocation = 0;
	GLint m_projectionMatrixLocation = 0;
	GLint m_densityLocation = 0;
	GLint m_gradientLocation = 0;
	GLint m_chunkPositionLocation = 0;
	GLint m_cameraPositionLocation = 0;
	
};

