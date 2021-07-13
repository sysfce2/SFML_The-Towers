#pragma once

#include <string>
#include "GUILabel.hpp"
#include "../Libs/stb_truetype.h"
#include "../Math/Math.hpp"


class GUIFont {
public:

	void init(const std::string& fontLocation, float pixelHeight, unsigned int w, unsigned int h, unsigned int firstChar = 32, unsigned int numChars = 95);
	void bindTexture();
	void unbindTexture();
	void updateMesh(GUILabel& mesh);
	void destroy();


private:

	void renderQuad(std::vector<GUITextVertex>& vertices, const math::vec4& destRect, const math::vec4& uv);

	unsigned int m_bitmapWidth = 0;
	unsigned int m_bitmapHeight = 0;
	stbtt_bakedchar* m_charData = nullptr;
	GLuint m_textureID = 0;

};
