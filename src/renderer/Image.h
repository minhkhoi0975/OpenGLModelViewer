#pragma once

#include <filesystem>

class Image
{
public:
	Image(const char* filePath, bool flipVertically = false);
	~Image();

	bool Load(const char* filePath, bool flipVertically = false);
	void Unload();

	inline std::filesystem::path GetFilePath() const {return filePath;}

	inline int GetWidth() const {return width;}
	inline int GetHeight() const {return height;}
	inline int GetChannelCount() const {return channelCount;}
	inline unsigned char* GetData() const {return data;}

private:
	std::filesystem::path filePath;

	int width, height;
	int channelCount;
	unsigned char* data = nullptr;
};