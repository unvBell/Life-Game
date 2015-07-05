#pragma once

#include <GLFW\glfw3.h>
#include "IRenderer.hpp"

class RendererGL final : public IRenderer {
	GLint texture_;
	GLint shaderLine_;
	GLint shaderTexture_;

public:
	explicit RendererGL();
	~RendererGL();

	void renderChunk(int x, int y, const Chunk& chunk);
};
