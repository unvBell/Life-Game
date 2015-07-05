#pragma once

class Chunk;

class IRenderer abstract {
public:
	virtual ~IRenderer() {}

	virtual void renderChunk(int x, int y, const Chunk& chunk) =0;
};
