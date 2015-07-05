#include "RendererGL.hpp"
#include "Chunk.hpp"

RendererGL::RendererGL()
	: texture_		(0)
	, shaderLine_	(0)
	, shaderTexture_(0)
{
}


RendererGL::~RendererGL() {
}


void RendererGL::renderChunk(int x, int y, const Chunk& chunk) {
	int offX = x*chunkSize;
	int offY = y*chunkSize;

	//	cell
	glBegin(GL_QUADS);
	for(int j=0; j<chunkSize; j++) {
		for(int i=0; i<chunkSize; i++) {
			if(chunk.get(i, j)) {
				glColor3d(0, 1, 0);

				glVertex2d(offX+i	, offY+j	);
				glVertex2d(offX+i+1	, offY+j	);
				glVertex2d(offX+i+1	, offY+j+1	);
				glVertex2d(offX+i	, offY+j+1	);
			}
		}
	}
	glEnd();
	
	//	grid
	glBegin(GL_LINES);
	glColor3d(0.7, 0.7, 0.7);
	for(int i=0; i<=chunkSize; i++) {
		glVertex2i(offX + i			, offY + 0			);
		glVertex2i(offX + i			, offY + chunkSize	);
		glVertex2i(offX + 0			, offY + i			);
		glVertex2i(offX + chunkSize	, offY + i			);
	}
	glEnd();
}
