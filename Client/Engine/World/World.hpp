#ifndef WORLD_H
#define WORLD_H
#include <Engine/World/Chunk.hpp>
#include <Engine/Libs/Frustum.hpp>
#include <Engine/Utils/Camera.hpp>
#include <Engine/Utils/ChunkShader.hpp>
#include <Engine/Utils/TextureArray.hpp>
#include <Settings.hpp>
#include <glm/gtc/noise.hpp>
#include <cstdint>



struct BlockTexture {
	BlockTexture(uint16_t _t){
		top = _t;
		side = _t;
		bot = _t;
	}
	BlockTexture(uint16_t _top, uint16_t _side, uint16_t _bot){
		top = _top;
		side = _side;
		bot = _bot;
	}
	uint16_t top = 0;
	uint16_t side = 0;
	uint16_t bot = 0;
};

class World {
public:

	void init(uint8_t* d);
	void render(Camera& camera);
	void destroy();
	uint8_t getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, uint8_t block);
	
	
private:

	void generateMesh(Chunk* chunk);
	Chunk* getChunk(int x, int y, int z);
	GLuint packData(uint8_t x, uint8_t y, uint8_t z, uint8_t lightLevel, uint8_t textureCoordinateIndex, uint8_t textureArrayIndex);
	void addBlock(Chunk* _c, int _x, int _y, int _z, uint8_t _blockType);
	BlockTexture getTextureFromBlockID(uint8_t _blockID);

	//Mesh generation functions
	void addTopFace(Chunk* _c, uint8_t _x, uint8_t _y, uint8_t _z, uint16_t _textureLayer);
	void addBottomFace(Chunk* c, uint8_t x, uint8_t y, uint8_t z, uint16_t _textureLayer);
	void addRightFace(Chunk* c, uint8_t x, uint8_t y, uint8_t z, uint16_t _textureLayer);
	void addLeftFace(Chunk* c, uint8_t x, uint8_t y, uint8_t z, uint16_t _textureLayer);
	void addFrontFace(Chunk* c, uint8_t x, uint8_t y, uint8_t z, uint16_t _textureLayer);
	void addBackFace(Chunk* c, uint8_t x, uint8_t y, uint8_t z, uint16_t _textureLayer);

	//We keep vertices so we dont have to reallocate memory every time we want to generate a chunk
	std::vector<GLuint> vertices;

	Chunk* chunks = nullptr;
	ChunkShader shader;
	uint8_t* data = nullptr;
	TextureArray texturePack;


};


#endif
