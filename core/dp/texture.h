//texture.h
#pragma once
namespace dp {
	unsigned int loadTexture(const char* filePath, int wrapMode, int filterMode);
	int getTextureFormat(int numComponents);
}